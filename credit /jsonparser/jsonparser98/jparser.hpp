#include <ostream>
#include <string>
#include <vector>

#ifndef JPARSER_H
#define JPARSER_H

namespace jsonparser {
    // Exception used when invalid JSON is encountered
    class invalid_json_exception : public std::invalid_argument {
    public:
        // Constructor
        inline invalid_json_exception(const char* error_message) : std::invalid_argument(error_message) {}
        // Destructor
        inline virtual ~invalid_json_exception() throw() {}
    };

    // Exception used for invalid JSON keys 
    class invalid_key_exception : public std::exception {
    public:
        const std::string key;
        // Constructor
        inline invalid_key_exception(const std::string& key) : key(key) {}
        // Destructor
        inline virtual ~invalid_key_exception() throw() {}
        inline virtual const char* what() const throw() { return key.c_str(); }
    };

    // Handle JSON data types
    namespace jtype {
        // JSON data types descriptor
        enum jtype {
            jstring,
            jnumber,
            jobject,
            jarray,
            jbool,
            jnull,
            jnotvalid
        };

        // Determines the type of value the string contain
        // parameter input is the string to be tested
        // returns the type of JSON value found
        // determinates the type of the first value found in the string
        jtype detect(const char* input);
    }
    
    // Functions used for JSON parsing will be here
    namespace parsing {
        // Return a pointer to the first character of the next value that is not a whitespace
        const char* delete_leading_white_space(const char* start);

        // Take an input string and read the digits at the front of the string until a characters other than 0-9 is encountered
        std::string read_digits(const char* input);

        // Escape quotes in string 
        std::string escape_char_quotes(const char* input);

        // Remove the escape character from quotes
        std::string remove_escaped_char_quotes(const char* input);

        // Capture the results of a JSON parsing
        struct parse_results {
            // Type of value encountered while parsing
            jtype::jtype type;
            // The parsed value encountered
            std::string value;
            // Pointer to the first character
            const char* firstCharacterPointer;
        };

        // Parse the first value in the string
        parse_results parse(const char* input);

        // Template for reading a numeric value 
        template<typename T> T get_number(const char* input, const char* format) {
            T result;
            // Read formatted data from string
            std::sscanf(input, format, &result);
            return result;
        }

        // Convert a number to a string
        template<typename T> std::string get_number_as_string(const T& number, const char* format) {
            std::vector<char> cstr(6);
            int firstCharacterPointer = std::snprintf(&cstr[0], cstr.size(), format, number);
            if(firstCharacterPointer < 0) {
                return std::string();
            } else if(firstCharacterPointer >= (int)cstr.size()) {
                cstr.resize(firstCharacterPointer + 1);
                std::snprintf(&cstr[0], cstr.size(), format, number);
            }
            std::string result(&cstr[0]);
            return result;
        }

        // Convert a serialized JSON array into a vector of the values in the array
        std::vector<std::string> parse_array(const char* input);
    }

    typedef std::pair<std::string, std::string> key_value_pair;

    // Class used for manipulating json objects and arrays
    class jobject {
    private:
        // Container used to store the objects data
        std::vector<key_value_pair> data;
        // Mark whether the object is valid array
        bool array_flag;    
    public:
        // Default constructor
        // array if true the instance is initialized as an array if false the instance is initilized as an object
        jobject(bool array = false): array_flag(array) {}
        // Copy constructor
        jobject(const jobject& other): data(other.data), array_flag(other.array_flag) {}
        // Destructor
        virtual ~jobject() {}
        // Flag for differentiating objects and arrays
        bool is_array() const { return this->array_flag; }
        // Number of entries in the json object or array
        size_t size() const { return this->data.size(); }
        // Clear the json object or array
        void clear() { this->data.resize(0); }
        // Comparison operator
        bool operator==(const jobject other) const { return((std::string)(*this)) == (std::string)other; }
        // Comparison operator
        bool operator!=(const jobject other) const { return((std::string)(*this)) != (std::string)other; }
        // Assignment operator
        jobject& operator=(const jobject& other) { this->data = other.data; this->array_flag = other.array_flag; return *this; }
        // Appends a key value pair to the json object 
        jobject& operator+=(const key_value_pair& other) { this->data.push_back(other); return *this; }
        // Appends one json object to another
        jobject& operator+=(const jobject& other) { this->data.insert(this->data.end(), other.data.begin(), other.data.end()); return *this; }
        // Merges two json objects
        jobject& operator+(const jobject& other) { *this += other; return *this; }
        // Parse a serialized json string
        static jobject parse(const char* input);
        // Parse a serialized json string
        static jobject parse(const std::string& input) { return parse(input.c_str()); }
        // Determine if an object has a key
        bool has_key(const std::string& key) const {
            if(this->array_flag) {
                return false;
            } 
            for(size_t i = 0; i < this->data.size(); i++) {
                if(this->data[i].first == key) {
                    return true;
                }
            }
            return false;
        }
        // Set the value assocaiated with a key
        void set_value(const std::string& key, const std::string& value);
        // Return the serialized value at a given index
        std::string get_value(size_t index) const {
            return this->data[index].second;
        }
        // Returns the serialized value associated with a key
        std::string get_value(const std::string& key) const {
            if(this->array_flag) {
                throw invalid_key_exception(key);
            }
            for(size_t i = 0; i < this->data.size(); i++) {
                if(this->data[i].first == key) {
                    return this->get_value(i);
                }
            }
            throw invalid_key_exception(key);
        }
        // Remove the entry associated with a key
        void remove_value(const std::string& key);
        // Remove the entry at the specified index
        void remove_value(size_t index) {
            this->data.erase(this->data.begin() + index);
        }
        // Representation of a value in the json object
        class jvalue {
        protected:
            // Method for reference the entrys value
            virtual const std::string& ref() const = 0;

            // Convert an serialzed json value to a numeric value
            template<typename T> T get_number(const char* format) const {
                return jsonparser::parsing::get_number<T>(this->ref().c_str(), format);
            }
            // Convert a serialized array of numbers to a vector of numbers
            template<typename T> std::vector<T> get_number_array(const char* format) const {
                std::vector<T> result;
                std::vector<std::string> array = jsonparser::parsing::parse_array(this->ref().c_str());
                for(size_t i = 0; i < array.size(); i++) {
                    result.push_back(jsonparser::parsing::get_number<T>(array[i].c_str(), format));
                }
                return result;
            }
        public:
            // Return a string representation of the value
            std::string as_string() const { 
                return jsonparser::parsing::remove_escaped_char_quotes(jsonparser::parsing::parse(this->ref().c_str()).value.c_str()); 
            }
            operator std::string() const {
                return this->as_string();
            }
            // Comparison operator
            bool operator==(const std::string other) const { return ((std::string)(*this)) == other; }
            // Comparison operator
            bool operator!=(const std::string other) const { return ((std::string)(*this)) == other; }
            // Cast the value as an integer
            operator int() const;
            // Cast the value as an unsigned integer
            operator unsigned int() const;
            // Cast the value as a long
            operator long() const;
            // Cast the value as an unsigned long
            operator unsigned long() const;
            // Cast the value as a char
            operator char() const;
            // Cast the value as float
            operator float() const;
            // Cast the value as double
            operator double() const;

            // Cast the value as a json object
            jsonparser::jobject as_object() const {
                return jsonparser::jobject::parse(this->ref().c_str());
            }
            // Cast the value as a json array
            operator jsonparser::jobject() const {
                return this->as_object();
            }

            // Cast the value as a json array of integers
            operator std::vector<int>() const;
            // Cast the value as a json array of unsigned integers
            operator std::vector<unsigned int>() const;
            // Cast the value as a json array of longs
            operator std::vector<long>() const;
            // Cast the value as a json array of unsigned longs
            operator std::vector<unsigned long>() const;
            // Cast the value as a json array of chars
            operator std::vector<char>() const;
            // Cast the value as a json array of floats
            operator std::vector<float>() const;
            // Cast the value as a json array of doubles
            operator std::vector<double>() const;
            // Cast an array of json objects to a vector of json objects
            operator std::vector<jsonparser::jobject>() const {
                const std::vector<std::string> array = jsonparser::parsing::parse_array(this->ref().c_str());
                std::vector<jsonparser::jobject> result;
                for(size_t i = 0; i < array.size(); i++) {
                    result.push_back(jsonparser::jobject::parse(array[i].c_str()));
                }
                return result;
            }
            // Cast an array of strings
            operator std::vector<std::string>() const {
                return jsonparser::parsing::parse_array(this->ref().c_str());
            }

            // Casts an array template
            template<typename T> std::vector<T> as_array() const {
                return (std::vector<T>)(*this);
            }

            // return true if the value is a null value
            bool is_null() const {
                return jsonparser::parsing::parse(this->ref().c_str()).type == jsonparser::jtype::jnull;
            }      
        };
        // Class to represent an entry as a constant value
        class jconst_value : public jvalue {
        private:
            // A copy of the entry data
            std::string data;

        protected:
            // Reference to a entry data
            const std::string& ref() const {
                return this->data;
            }
        public:
            // Construct a proxy with the provided value
            jconst_value(const std::string& value) : data(value) {}
            // Return another constant value from this object
            jconst_value get(const std::string& key) const {
                return jconst_value(jsonparser::jobject::parse(this->data).get_value(key));
            }
            // Return another constant value from this array
            jconst_value array(size_t index) const {
                return jconst_value(jsonparser::jobject::parse(this->data).get_value(index));
            }
        };
        // Class to represent an entry as a constant proxy to the value
        class jconst_proxy : public jvalue {
        private:
            // The source object the value is referencing
            const jsonparser::jobject& source;

        protected:
            const std::string key;
            // Reference to a entry data
            const std::string& ref() const {
                for(size_t i = 0; i < this->source.size(); i++) {
                    if(this->source.data.at(i).first == key) {
                        return this->source.data.at(i).second;
                    }
                }
                throw jsonparser::invalid_key_exception(key);
            }
        public:
            // Construct a proxy with the provided value
            jconst_proxy(const jobject& source, const std::string key) : source(source), key(key) {
                if(source.array_flag) {
                    throw std::logic_error("cannot use array() on a constant object");
                }
            }
            // Return another constant value from this array
            jconst_value array(size_t index) const {
                const char* value = this->ref().c_str();
                if(jsonparser::jtype::detect(value) != jsonparser::jtype::jarray) {
                    throw jsonparser::invalid_json_exception("cannot use array() on a non-array value");
                }
                const std::vector<std::string> array = jsonparser::parsing::parse_array(value);
                return jconst_value(array[index]);
            }
        };
        // Proxy class that allows modification of the value
        class jproxy : public jsonparser::jobject::jconst_proxy {
        private:
            // The source object the value is referencing
            jsonparser::jobject& source_object;

        protected:
            // Template sets a number value in the parent object
            template<typename T> void set_number(const T value, const char* format) {
                this->source_object.set_value(this->key, jsonparser::parsing::get_number_as_string(value, format));
            }

            // Store an array of values
            void set_value_array(const std::vector<std::string>& values, const bool wrap = false);

            // Template stores an array of numbers
            template<typename T> void set_number_array(const std::vector<T>& values, const char* format){
                std::vector<std::string> array;
                for(size_t i = 0; i < values.size(); i++) {
                    array.push_back(jsonparser::parsing::get_number_as_string(values[i], format));
                }
                this->set_value_array(array);
            }
            
        public:
            // Constructor
            jproxy(jsonparser::jobject& source, const std::string key) :jsonparser::jobject::jconst_proxy(source, key), source_object(source) {}
            // Assings a string value
            void operator= (const std::string value) {
                this->source_object.set_value(this->key, "\"" + jsonparser::parsing::escape_char_quotes(value.c_str()) + "\"");
            }
            // Assings a string value
            void operator= (const char* value) {
                this->operator=(std::string(value));
            }
            // Assings a number value
            void operator= (const int input) {
                this->set_number(input, "%i");
            }
            // Assing an unsigned number value
            void operator= (const unsigned int input) {
                this->set_number(input, "%u");
            }
            // Assign a long value
            void operator= (const long input) {
                this->set_number(input, "%li");
            }
            // Assign an unsigned long value
            void operator= (const unsigned long input) {
                this->set_number(input, "%lu");
            }
            // Assign a charater value
            void operator= (const char input) {
                this->set_number(input, "%c");
            }
            // Assign a float value
            void operator= (const float input) {
                this->set_number(input, "%e");
            }
            // Assign a double value
            void operator= (const double input) {
                this->set_number(input, "%e");
            }
            // Assign a json object or array
            void operator= (const jsonparser::jobject& input) {
                this->source_object.set_value(this->key, (std::string) input);
            }

            // Assign an array of integers
            void operator= (const std::vector<int>& input) {
                this->set_number_array(input, "%i");
            }
            // Assign an array of unsigned integers
            void operator= (const std::vector<unsigned int>& input) {
                this->set_number_array(input, "%u");
            }
            // Assign an array of longs
            void operator= (const std::vector<long>& input) {
                this->set_number_array(input, "%li");
            }
            // Assign an array of unsigned longs
            void operator= (const std::vector<unsigned long>& input) {
                this->set_number_array(input, "%lu");
            }
            // Assign an array of characters
            void operator= (const std::vector<char>& input) {
                this->set_number_array(input, "%c");
            }
            // Assign an array of floats
            void operator= (const std::vector<float>& input) {
                this->set_number_array(input, "%f");
            }
            // Assign an array of doubles
            void operator= (const std::vector<double>& input) {
                this->set_number_array(input, "%lf");
            }
            // Assign an array of strings
            void operator= (const std::vector<std::string>& input) {
                this->set_value_array(input, true);
            }
            // Assign an array of json objects or arrays
            void operator= (const std::vector<jsonparser::jobject>& input) {
                std::vector<std::string> array;
                for(size_t i = 0; i < input.size(); i++) {
                    array.push_back((std::string) input[i]);
                }
                this->set_value_array(array);
            }
            // Sets a boolean value
            void set_boolean(const bool value) {
                this->source_object.set_value(key, value ? "true" : "false");
            }
            // Sets a null value
            void set_null() {
                this->source_object.set_value(key, "null");
            }
            // Clears the value
            void clear() {
                this->source_object.set_value(key, "");
            }
        };
        // Return an element of the json object
        jobject::jproxy operator[] (const std::string key) {
            if(this->array_flag) {
                throw jsonparser::invalid_key_exception(key);
            }
            return jobject::jproxy(*this, key);
        }
        // Return an element of the json object
        jobject::jconst_proxy operator[] (const std::string key) const {
            if(this->array_flag) {
                throw jsonparser::invalid_key_exception(key);
            }
            return jobject::jconst_proxy(*this, key);
        }
        // Return the value of an elemnt in an array
        jobject::jconst_value array(const size_t index) const {
            return jobject::jconst_value(this->data[index].second);
        }
        operator std::string() const;
        // Serialize the  object or array
        std::string serialize() const {
            return this->operator std::string();
        }
    };
}  

#endif
