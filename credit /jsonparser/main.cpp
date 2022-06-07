#include "jparser.hpp"
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
    std::vector<std::string> tests = {
        "null",
        "true",
        "false",
        " \n\t null \t",
        "[]",
        "[null]",
        "[true, false]",
        "[true, false, null]",
        "[true,    false,     null,true   ]",
        "{}",
        "{\"a\": null}",
        "{\"a\": true, \"b\": false, \"c\": null}",
        "{\"a\": [true, false, null], \"b\": {\"a\": null}}",
        "123.456",
        "123.456e7",
        "123.456e-7",
        "-123.456e7",
        "-123.456e+7",
        "\"There are UTF8 codepoints among us: \\u0d9E\"",
    };

    for (auto test : tests)
    {
        cout << "Parsing: " << test << endl;
        auto result = jsonparser::parse(test);
        cout << "Serialized: " << jsonparser::serialize(result) << endl;
    }

    // This could come e.g. from a file
    std::string json_config = R"(
        {
            "flag": true,
            "array": [
                null,
                123,
                -123e+2,
                "str \u0d9E"
            ],
            "key": "value",
            "nested": {
                "key": "value"
            }
        }
        )";

    jsonparser::Json config = jsonparser::parse(json_config);
    bool flag_value = std::get<bool>(std::get<jsonparser::JSObject>(config)["flag"]);
    cout << "Flag value: " << flag_value << endl;

    jsonparser::Json json_str = std::string("string"); // JSString
    jsonparser::Json json_number = (double)123;        // JSNumber
    jsonparser::Json json_bool = true;                 // JSBool
    jsonparser::Json json_null = nullptr;              // JSNull
    jsonparser::JSArray arr = {true, nullptr, "str", (double)123, 123.456e-1};
    jsonparser::JSObject obj = {
        {"key", "value"},
        {"array", arr},
        {"empty_array", jsonparser::JSArray()},
        {"empty_object", jsonparser::JSObject()}};
    cout << "Serialized: " << jsonparser::serialize(obj) << endl;
}