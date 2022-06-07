#include "jparser.hpp"
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))

using namespace std;

int main()
{
    // counter of passed tests
    unsigned long passed = 0;

    vector<pair<string, string>> tests = {
        {"true", "true"},
        {"false", "false"},
        {"null", "null"},
        {" \n\t null \t", "null"},
        {"[]", "[]"},
        {"[null]", "[null]"},
        {"[true, false]", "[true, false]"},
        {"[true, false, null]", "[true, false, null]"},
        {"[true,    false,     null,true   ]", "[true, false, null, true]"},
        {"{}", "{}"},
        {"{\"a\": null}", "{\"a\": null}"},
        {"123", "123"},
        {"123.456", "123.456"},
        {"123.456e1", "1234.56"},
        {"123.456e-1", "12.3456"},
        {"-123.456e1", "-1234.56"},
        {"-123.456e+1", "-1234.56"},
        {"\"There are UTF8 codepoints among us: \\u0d9E\"", "\"There are UTF8 codepoints among us: ඞ\""},
    };

    for (auto [test, expected] : tests)
    {
        try
        {
            auto json = jsonparser::parse(test);
            auto serialized = jsonparser::serialize(json);
            if (serialized != expected)
            {
                cout << "received: " << test << ", got " << serialized << ", expected " << expected << endl;
            }
            if (serialized == expected)
            {
                passed++;
            }
        }
        catch (const exception &e)
        {
            cout << test << endl;
            cout << "Parse Error: " << e.what() << endl;
        }
    }

    vector<string> fails = {
        "tru",
        "fals",
        "\"string_with_no_closing_quote",
        "123.",
        "123.e",
        "0123",
        "[",
        "[12, 34, }",
    };

    for (auto fail : fails)
    {
        try
        {
            auto json = jsonparser::parse(fail);
            auto serialized = jsonparser::serialize(json);
            cout << "received: " << fail << ", got " << serialized << ", expected error" << endl;
        }
        catch (const jsonparser::ParsingError &e)
        {
            passed++;
            continue;
        }
    }
    cout << "Passed " << passed << " out of " << tests.size()+fails.size() << " tests" << endl;

    // when any test fails, the program will exit with code 1
    if (passed != (tests.size()+fails.size()))
    {
        exit(1);
    }
    
}
