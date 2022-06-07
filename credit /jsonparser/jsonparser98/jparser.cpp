#include "jparser.hpp"
#include <string.h>

using namespace std;

// Check for an empty string
#define EMPTY_STRING(str)(*str == '\0')

// Move the pointer to the first character that is not white space
#define SKIP_WHITESPACE(str){const char* next = jsonparser::parsing::delete_leading_white_space(str); str = next;}

// Determines if the end character or serialized JSOn is reached
#define END_CHARACTER(obj, index)(obj.is_array() ? *index == ']' : *index == '}')

// Delete the leading white spacea
const char* jsonparser::parsing::delete_leading_white_space(const char* str) {
    while(*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r' || *str == '\v' || *str == '\f') {
        str++;
    }
    return str;
}

// Formats for conversion
const char* INT_FORMAT = "%i";          // integer to string
const char* UINT_FORMAT = "%u";         // unsigned integer to string
const char* LONGINT_FORMAT = "%li";     // long integer to string
const char* ULONGINT_FORMAT = "%lu";    // unsigned long integer to string
const char* CHAR_FORMAT = "%c";         // character to string
const char* FLOAT_FORMAT = "%f";        // floating-point number to string
const char* DOUBLE_FORMAT = "%lf";      // double floating-point number to string

jsonparser::jtype::jtype jsonparser::jtype::detect(const char* input) {
    const char* start = jsonparser::parsing::delete_leading_white_space(input);
    if(EMPTY_STRING(start)){
        return jsonparser::jtype::jnotvalid;
    }
    switch (*start)
    {
    case '"':
        return jsonparser::jtype::jstring;
    case '[':
        return jsonparser::jtype::jarray;
    case '{':
        return jsonparser::jtype::jobject;
    case 't':
        return jsonparser::jtype::jbool;
    case 'f':
        return jsonparser::jtype::jbool;
    case 'n':
        return jsonparser::jtype::jnull;
    default:
        if(isdigit(*start) || *start == '-') {
            return jsonparser::jtype::jnumber;
        }
        return jsonparser::jtype::jnotvalid;
    }
}

// Escape quotes in string
string jsonparser::parsing::escape_char_quotes(const char* input) {
    string output;
    while(!EMPTY_STRING(input)) {
        if(*input == '"') {
            output += '\\';
        }
        output += *input;
        input++;
    }
    return output;
}

// Remove the escape character from quotes
string jsonparser::parsing::remove_escaped_char_quotes(const char* input) {
    string parsed_result;
    const size_t len = strlen(input);
    for(size_t i = 0; i < len; i++){
        if(input[i] == '\\' && input[i+1] == '\"') {
            i++;
        }
        parsed_result += input[i];
    }
    return parsed_result;
}

string jsonparser::parsing::read_digits(const char* input) {
    const char* index = jsonparser::parsing::delete_leading_white_space(input);
    string result;
    while(!EMPTY_STRING(index)) {
        if(*index >= '0' && *index <= '9') {
            result += *index;
            index++;
        } else {
            break;
        }
    }
    return result;
}

jsonparser::parsing::parse_results jsonparser::parsing::parse(const char* input) {
    const char* index = jsonparser::parsing::delete_leading_white_space(input);
    // Validate input
    if(EMPTY_STRING(index)) {
        throw jsonparser::invalid_json_exception("Error: Empty string");
    }
    jsonparser::parsing::parse_results result;
    result.type = jsonparser::jtype::detect(index);

    switch (result.type) {
    case jsonparser::jtype::jstring:
    {
        const char error_message[] = "Error: String is not properly formatted - '\"' expected!";
        if(*index != '\"') { 
            throw jsonparser::invalid_json_exception(error_message);
        }
        index++;

        while(!EMPTY_STRING(index)) {
            if(*index != '"' || *(index-1) == '\\') {
                result.value += *index;
                index++;
            } else {
                break;
            }
        }
        if(EMPTY_STRING(index) || *index != '"') {
            result.type = jsonparser::jtype::jnotvalid;
        } else {
            index++;
        }
        break;
    }
    case jsonparser::jtype::jnumber:
    {
        const char error_message[] = "Error: Invalid number";
        if(*index == '-') {
            result.value.push_back('-');
            index++;
        }
        if(EMPTY_STRING(index)) {
            throw jsonparser::invalid_json_exception(error_message);
        }
        // Read the digits
        string whole_digits = jsonparser::parsing::read_digits(index);
        
        // Validate the read
        if(whole_digits.length() == 0) {
            throw jsonparser::invalid_json_exception(error_message);
        }

        result.value += whole_digits;
        index += whole_digits.length();

        if (*index == '.'){
            result.value.push_back('.');
            index++;
            string decimal_digits = jsonparser::parsing::read_digits(index);
            if(decimal_digits.length() == 0) {
                throw jsonparser::invalid_json_exception(error_message);
            }
            result.value += decimal_digits;
            index += decimal_digits.size();
        }
        break;
    }
    case jsonparser::jtype::jobject:
    {
        const char error_message[] = "Error: Invalid object";
        // First character must be an '{'
        if(*index != '{') {
            throw jsonparser::invalid_json_exception(error_message);
        }
        result.value += '{';
        index++;
        SKIP_WHITESPACE(index);

        while(!EMPTY_STRING(index) && *index != '}') {
            // Read the key
            jsonparser::parsing::parse_results key = jsonparser::parsing::parse(index);
            // Validate that the key is a string
            if(key.type != jsonparser::jtype::jstring) {
                throw jsonparser::invalid_json_exception(error_message);
            }
            // Store the key
            result.value += "\"" + jsonparser::parsing::escape_char_quotes(key.value.c_str()) + "\"";
            index = jsonparser::parsing::delete_leading_white_space(key.firstCharacterPointer);

            // Look for the colon
            if(*index != ':') {
                throw jsonparser::invalid_json_exception(error_message);
            }
            result.value.push_back(':');
            index++;
            // Get the value
            jsonparser::parsing::parse_results value = jsonparser::parsing::parse(index);
            // Validate
            if(value.type == jsonparser::jtype::jnotvalid) {
                throw jsonparser::invalid_json_exception(error_message);
            }
            // Store the value
            if(value.type == jsonparser::jtype::jstring) {
                result.value += "\"" + jsonparser::parsing::escape_char_quotes(value.value.c_str()) + "\"";
            } else {
                result.value += value.value;
            }
            index = jsonparser::parsing::delete_leading_white_space(value.firstCharacterPointer);
            // Validate format
            if(*index != ',' && *index != '}') {
                throw jsonparser::invalid_json_exception(error_message);
            }
            // Look for the comma
            if(*index == ',') {
                result.value.push_back(',');
                index++;
            }
        }
        // Validate format
        if (*index != '}') {
            throw jsonparser::invalid_json_exception(error_message);
        }
        result.value += '}';
        index++;
        break;
    }
    case jsonparser::jtype::jarray:
    {
        const char error_message[] = "Error: Invalid array";
        if(*index != '[') {
            throw jsonparser::invalid_json_exception(error_message);
        }
        result.value += '[';
        index++;
        SKIP_WHITESPACE(index);

        if(EMPTY_STRING(index)) {
            throw jsonparser::invalid_json_exception(error_message);
        }

        while(!EMPTY_STRING(index) && *index != ']') {
            jsonparser::parsing::parse_results array_val = jsonparser::parsing::parse(index);
            if(array_val.type == jsonparser::jtype::jnotvalid) {
                throw jsonparser::invalid_json_exception(error_message);
            }
            if(array_val.type == jsonparser::jtype::jstring) {
                result.value += "\"" + jsonparser::parsing::escape_char_quotes(array_val.value.c_str()) + "\"";
            } else {
                result.value += array_val.value;
            }
            index = jsonparser::parsing::delete_leading_white_space(array_val.firstCharacterPointer);
            if(*index != ',' && *index != ']') {
                throw jsonparser::invalid_json_exception(error_message);
            }
            if(*index == ',') {
                result.value.push_back(',');
                index++;
            }
        }

        if(*index != ']') {
            throw jsonparser::invalid_json_exception(error_message);
        }

        result.value.push_back(']');
        index++;
        break;
    }
    case jsonparser::jtype::jbool:
    {
        const char error_message[] = "Error: Invalid boolean";
        if(strncmp(index, "true", 4) == 0) {
            result.value += "true";
            index += 4;
        }
        else if(strncmp(index, "false", 4) == 0) {
            result.value += "false";
            index += 5;
        } else {
            throw jsonparser::invalid_json_exception(error_message);
        }

        break;
    }
    case jsonparser::jtype::jnull:
    {
        const char error_message[] = "Error: Invalid null";
        if(strncmp(index, "null", 4) == 0) {
            result.value += "null";
            index += 4;
        } else {
            throw jsonparser::invalid_json_exception(error_message);
        }
        break;
    }
    default:
        throw jsonparser::invalid_json_exception("Error: Invalid JSON object");
        break;
    }

    result.firstCharacterPointer = index;
    return result;
}

vector<string> jsonparser::parsing::parse_array(const char* input) {
    vector<string> result;
    const char* index = jsonparser::parsing::delete_leading_white_space(input);
    if(*index != '[') {
        throw jsonparser::invalid_json_exception("Error: The input didn't contain a valid JSON array.");
    }
    index++;
    SKIP_WHITESPACE(index);
    if(*index == ']'){
        return result;
    }
    const char error_message[] = "Error: The input was NOT properly formatted.";
    while(!EMPTY_STRING(index)) {
        SKIP_WHITESPACE(index);
        jsonparser::parsing::parse_results array_val = jsonparser::parsing::parse(index);
        if(array_val.type == jsonparser::jtype::jnotvalid) {
            throw jsonparser::invalid_json_exception(error_message);
        }
        result.push_back(array_val.value);
        index = jsonparser::parsing::delete_leading_white_space(array_val.firstCharacterPointer);
        if (*index == ']') {
            break;
        }
        if(*index == ',') {
            index++;
        }
    }
    if (*index != ']') {
        throw jsonparser::invalid_json_exception(error_message);
    }
    index++;
    return result;
}

jsonparser::jobject jsonparser::jobject::parse(const char* input) {
    const char error_message[] = "Error: The input didn't contain a valid JSON object.";
    jsonparser::jobject result;
    const char* index = jsonparser::parsing::delete_leading_white_space(input);
    switch(*index) {
    case '[':
        result = jsonparser::jobject(true);
        break;
    case '{':
        break;
    default:
        throw jsonparser::invalid_json_exception(error_message);
        break;
    }
    index++;
    SKIP_WHITESPACE(index);
    if(EMPTY_STRING(index)) {
        throw jsonparser::invalid_json_exception(error_message);
    }

    while(!EMPTY_STRING(index) && !END_CHARACTER(result, index)) {
        // get the key
        key_value_pair entry;
        if(!result.is_array()) {
            jsonparser::parsing::parse_results key = jsonparser::parsing::parse(index);
            if(key.type != jsonparser::jtype::jstring || key.value == "") {
                throw jsonparser::invalid_json_exception(error_message);
            }
            entry.first = key.value;
            index = key.firstCharacterPointer;
            SKIP_WHITESPACE(index);
            if(*index != ':') {
                throw jsonparser::invalid_json_exception(error_message);
            }
            index++;
        }

        SKIP_WHITESPACE(index);
        // gets value
        jsonparser::parsing::parse_results value = jsonparser::parsing::parse(index);
        // validate the value type
        if(value.type == jsonparser::jtype::jnotvalid) {
            throw jsonparser::invalid_json_exception(error_message);
        }
        if(value.type == jsonparser::jtype::jstring) {
            entry.second = "\"" + value.value + "\"";
        } else {
            entry.second = value.value;
        }
        index = value.firstCharacterPointer;

        // look for the comma
        SKIP_WHITESPACE(index);
        if(*index != ',' && !END_CHARACTER(result, index)) {
            throw jsonparser::invalid_json_exception(error_message);
        }
        if(*index == ',') {
            index++;
        }
        result += entry;
    }

    if(EMPTY_STRING(index) || !END_CHARACTER(result, index)) {
        throw jsonparser::invalid_json_exception(error_message);
    }
    index++;
    return result;
}

void jsonparser::jobject::set_value(const string& key, const string& value) {
    if(this->array_flag) {
        throw jsonparser::invalid_key_exception(key);
    }
    for (size_t i = 0; i < this->size(); i++) {
        if(this->data[i].first == key) {
            this->data[i].second = value;
            return;
        }
    }

    key_value_pair entry;
    entry.first = key;
    entry.second = value;
    this->data.push_back(entry);
}

void jsonparser::jobject::remove_value(const string& key) {
    if(this->array_flag) {
        throw jsonparser::invalid_key_exception(key);
    }
    for (size_t i = 0; i < this->size(); i++) {
        if(this->data[i].first == key) {
            this->remove_value(i);
        }
    }
}

void jsonparser::jobject::jproxy::set_value_array(const vector<string>& values, const bool wrap) {
    string value = "[";
    for (size_t i = 0; i < values.size(); i++) {
        if (wrap) {
            value += "\"" + jsonparser::parsing::escape_char_quotes(values[i].c_str()) + "\",";
        } else { 
            value += values[i] + ",";
        }
    }
    if(values.size() > 0) {
        value.erase(value.size() - 1, 1);
    }
    value += "]";
    this->source_object.set_value(key, value);
}

// operator int
jsonparser::jobject::jvalue::operator int() const {
    return this->get_number<int>(INT_FORMAT);
}
// operator unsigned int
jsonparser::jobject::jvalue::operator unsigned int() const {
    return this->get_number<unsigned int>(UINT_FORMAT);
}
// operator long
jsonparser::jobject::jvalue::operator long() const {
    return this->get_number<long>(LONGINT_FORMAT);
}
// operator unsigned long
jsonparser::jobject::jvalue::operator unsigned long() const {
    return this->get_number<unsigned long>(ULONGINT_FORMAT);
}
// operator char
jsonparser::jobject::jvalue::operator char() const {
    return this->get_number<char>(CHAR_FORMAT);
}
// operator float
jsonparser::jobject::jvalue::operator float() const {
    return this->get_number<float>(FLOAT_FORMAT);
}
// operator double
jsonparser::jobject::jvalue::operator double() const {
    return this->get_number<double>(DOUBLE_FORMAT);
}

// operator int array
jsonparser::jobject::jvalue::operator std::vector<int>() const {
    return this->get_number_array<int>(INT_FORMAT);
}
// operator unsigned int array
jsonparser::jobject::jvalue::operator std::vector<unsigned int>() const {
    return this->get_number_array<unsigned int>(UINT_FORMAT);
}
// operator long array
jsonparser::jobject::jvalue::operator std::vector<long>() const {
    return this->get_number_array<long>(LONGINT_FORMAT);
}
// operator unsigned long array
jsonparser::jobject::jvalue::operator std::vector<unsigned long>() const {
    return this->get_number_array<unsigned long>(ULONGINT_FORMAT);
}
// operator char array
jsonparser::jobject::jvalue::operator std::vector<char>() const {
    return this->get_number_array<char>(CHAR_FORMAT);
}
// operator float array
jsonparser::jobject::jvalue::operator std::vector<float>() const {
    return this->get_number_array<float>(FLOAT_FORMAT);
}
// operator double array
jsonparser::jobject::jvalue::operator std::vector<double>() const {
    return this->jsonparser::jobject::jvalue::get_number_array<double>(DOUBLE_FORMAT);
}

jsonparser::jobject::operator string() const {
    if(is_array()) {
        if(this->size() == 0) {
            return "[]";
        }
        string result = "[";
        for(size_t i = 0; i < this->size(); i++) {
            result += this->data[i].second + ",";
        }
        result.erase(result.size() - 1, 1);
        result += "]";
        return result;
    } else {
        if(this->size() == 0) { 
            return "{}";
        }
        string result = "{";
        for(size_t i = 0; i < this->size(); i++) {
            result += "\"" + this->data[i].first + "\"" + ":" + this->data[i].second + ",";
        }
        result.erase(result.size() - 1, 1);
        result += "}";
        return result;
    }
}
