# JSON Parser
## Specification, requirements
Requires C++17.  
Json specification used: https://www.json.org/json-en.html

## Build 
Build JSON parser library via Makefile:
```
make
```
`make` command will build [main.cpp](main.cpp).  
To get the output use:
```
./out
```

Clean via Makefile:
```
make clean
```

## Tests
To test JSON parser functionality, run tests created in [test.cpp](test.cpp) via Makefile:

```
make test
./test
```

# Parsing
Json value is obtained by calling the `parse(const std::string_view)` method. It is a variant (std::variant) containing these types:

- JSString as std::string
- JSArray as std::vector<Json>
- JSObject as std::unordered_map<JSString, Json>
- JSNumber as double
- JSBool as bool
- JSNull as nullptr_t

```cpp
using namespace std;
jsonparser::Json json = jsonparser::parse(R"(
    {
        "key": "value"
    }
)")

Json key_value = get<JSObject>(json)["key"];
string raw_value = get<JSString>(key_value); // Equals to "value"
```

During the construction of the Json object, a ParsingError exception can be thrown. Inspect it's message to see the reason why the parsing failed. The four possible modes of 
failure are:

- Unexpected end of JSON string (e.g. mismatching brackets)
- Unexpected token (e.g typo in a keyword)
- Unexpected escaped character (the standard allows only for a selected few characters to be escaped with "\")
- Number not being able to be stored in a double without precision loss (e.g. 1e10000)

# Serializing
Simply call the `jsonparser::serialize(const Json &value)` function to obtain a std::string.

```cpp
using namespace std;
// Initialization of the std::vector<Json> behind the JSArray type
jsonparser::Json array = {true, nullptr, "str"};
// Equals [true, null, "str"]
string serialized = jsonparser::serialize(serialized)
```

Note that the Json variant values can also be easily constructed using the default constructors of the respective types.
```cpp
jsonparser::Json json_str = std::string("string"); // JSString
jsonparser::Json json_number = (double)123 // JSNumber
jsonparser::Json json_bool = true; // JSBool
jsonparser::Json json_null = nullptr; // JSNull
jsonparser::Json array = {true, nullptr, "str"}; // JSArray
jsonparser::Json json_obj = { { "flag" : true}, {"array": array}};
```
