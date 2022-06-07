#include "jparser.hpp"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

// Tests

// Test function if the results value is equal to the expected results value
void test_value(const char* expected, const char* actual) {
    if(strcmp(expected, actual) != 0) {
       printf("%s[%s:%d]: Value test failed! %s != %s \n",
            __FUNCTION__, __FILE__, __LINE__, expected, actual);
       exit(1);
    }
}

// Test function if the results type is equal to the expected results type
void test_type(jsonparser::jtype::jtype expected, jsonparser::jtype::jtype actual) {
    if(expected != actual) {
       printf("%s[%s:%d]: Type test failed! %d != %d \n",
            __FUNCTION__, __FILE__, __LINE__, expected, actual);
       exit(1);
    }
}

// String parsing
void parse_string(){
    // "input":           "abc123 \""
    const char* input = " \t \n \v \f \r \"abc123 \\\"\"";
    jsonparser::parsing::parse_results result = jsonparser::parsing::parse(input);
    const char *expected = "abc123 \\\"";

    test_type(jsonparser::jtype::jstring, result.type);
    printf("%s[%s:%d]: Type test passed! \n",
        __FUNCTION__, __FILE__, __LINE__);

    test_value(expected, result.value.c_str());
    printf("%s[%s:%d]:\n expected: %s\n actual: %s\n Value test passed!\n",
        __FUNCTION__, __FILE__, __LINE__, expected, result.value.c_str());
}

void parse_string2(){
    const char* input = " \"hello\"";
    jsonparser::parsing::parse_results result = jsonparser::parsing::parse(input);
    const char* expected = "hello";

    test_type(jsonparser::jtype::jstring, result.type);
    printf("%s[%s:%d]: Type test passed! \n",
        __FUNCTION__, __FILE__, __LINE__);

    test_value(expected, result.value.c_str());
    printf("%s[%s:%d]:\n expected: %s\n actual: %s\n Value test  passed!\n",
        __FUNCTION__, __FILE__, __LINE__, expected, result.value.c_str());
}

// Number parsing
void parse_number(){
    const char* numbers[] = { "123", "123.456", "-123", "-123.456" };
    for(int i = 0; i < sizeof(numbers)/sizeof(*numbers); i++) {
        const char* input = numbers[i];
        jsonparser::parsing::parse_results result = jsonparser::parsing::parse(input);
        const char* expected = numbers[i];

        test_type(jsonparser::jtype::jnumber, result.type);
        printf("%s[%s:%d]: Type test passed! \n",
            __FUNCTION__, __FILE__, __LINE__);

        test_value(expected, result.value.c_str());
        printf("%s[%s:%d]:\n expected: %s\n actual: %s\n",
            __FUNCTION__, __FILE__, __LINE__, expected, result.value.c_str());
    }
    printf("Value test passed!\n");
}

void reading_digits(){
	const char* input = " 123457890";
	test_value("123457890", jsonparser::parsing::read_digits(input).c_str());
    //print the result
    printf("%s[%s:%d]:\n expected: %s\n actual: %s\n Value test passed!\n",
        __FUNCTION__, __FILE__, __LINE__, "123457890", jsonparser::parsing::read_digits(input).c_str());
}

void reading_digits2(){
    const char* input = " 123457890a";
	test_value("123457890", jsonparser::parsing::read_digits(input).c_str());
    //print the result
    printf("%s[%s:%d]:\n expected: %s\n actual: %s\n Value test passed!\n",
        __FUNCTION__, __FILE__, __LINE__, "123457890", jsonparser::parsing::read_digits(input).c_str());
}

// Object parsing
void parse_object(){
    const char* input = " {\"abc\":\"def\", \"123\":\"456\"}";
    jsonparser::parsing::parse_results result = jsonparser::parsing::parse(input);
    const char* expected = "{\"abc\":\"def\",\"123\":\"456\"}";

    test_type(jsonparser::jtype::jobject, result.type);
    printf("%s[%s:%d]: Type test passed! \n",
        __FUNCTION__, __FILE__, __LINE__);

    test_value(expected, result.value.c_str());
    printf("%s[%s:%d]:\n expected: %s\n actual: %s\n Value test passed!\n",
        __FUNCTION__, __FILE__, __LINE__, expected, result.value.c_str());
}

// Array parsing
void parse_array(){
    const char* input = " [{\"abc\":\"def\"}, {\"abc\":\"def\"}, {\"abc\":\"def\"}]";
    jsonparser::parsing::parse_results result = jsonparser::parsing::parse(input);
    const char* expected = "[{\"abc\":\"def\"},{\"abc\":\"def\"},{\"abc\":\"def\"}]";

    test_type(jsonparser::jtype::jarray, result.type);
    printf("%s[%s:%d]: Type test passed! \n",
        __FUNCTION__, __FILE__, __LINE__);

    test_value(expected, result.value.c_str());
    printf("%s[%s:%d]:\n expected: %s\n actual: %s\n Value test passed!\n",
        __FUNCTION__, __FILE__, __LINE__, expected, result.value.c_str());

}

// Empty array parsing
void parse_empty_array(){
    const char* input = " []";
    jsonparser::parsing::parse_results result = jsonparser::parsing::parse(input);
    const char* expected = "[]";

    test_type(jsonparser::jtype::jarray, result.type);
    printf("%s[%s:%d]: Type test passed! \n",
        __FUNCTION__, __FILE__, __LINE__);

    test_value(expected, result.value.c_str());
    printf("%s[%s:%d]:\n expected: %s\n actual: %s\n Value test passed!\n",
        __FUNCTION__, __FILE__, __LINE__, expected, result.value.c_str());
}

// String array parsing
void parse_string_array(){
    const char* input = " [\"world\"]},";
    jsonparser::parsing::parse_results result = jsonparser::parsing::parse(input);
    const char* expected = "[\"world\"]";

    test_type(jsonparser::jtype::jarray, result.type);
    printf("%s[%s:%d]: Type test passed! \n",
        __FUNCTION__, __FILE__, __LINE__);

    test_value(expected, result.value.c_str());
    printf("%s[%s:%d]:\n expected: %s\n actual: %s\n Value test passed!\n",
        __FUNCTION__, __FILE__, __LINE__, expected, result.value.c_str());
}

// Boolean parsing
void parse_bool(){
    const char* bools[] = { "true", "false" };
    for(int i = 0; i < sizeof(bools) / sizeof(bools[0]); i++) {
        const char* input = bools[i];
        jsonparser::parsing::parse_results result = jsonparser::parsing::parse(input);
        const char* expected = bools[i];

        test_type(jsonparser::jtype::jbool, result.type);
        printf("%s[%s:%d]: Type test passed! \n",
            __FUNCTION__, __FILE__, __LINE__);

        test_value(expected, result.value.c_str());
        printf("%s[%s:%d]:\n expected: %s\n actual: %s\n",
            __FUNCTION__, __FILE__, __LINE__, expected, result.value.c_str());
    }
    printf("Value test passed!\n");
}

// Null parsing
void parse_null(){
    const char* input = "  null";
    jsonparser::parsing::parse_results result = jsonparser::parsing::parse(input);
    const char* expected = "null";

    test_type(jsonparser::jtype::jnull, result.type);
    printf("%s[%s:%d]: Type test passed! \n",
        __FUNCTION__, __FILE__, __LINE__);

    test_value(expected, result.value.c_str());
    printf("%s[%s:%d]:\n expected: %s\n actual: %s\n Value test passed!\n",
        __FUNCTION__, __FILE__, __LINE__, expected, result.value.c_str());
}

// Whitespace parsing
void parse_whitespace(){
    const char* input = " \t \n \v \f \r ";
    const char* expected = "";
    test_value(expected, jsonparser::parsing::delete_leading_white_space(input));
    printf("%s[%s:%d]:\n expected: %s\n actual: %s\n Test passed!\n",
        __FUNCTION__, __FILE__, __LINE__, expected, jsonparser::parsing::delete_leading_white_space(input));
}

void parse_whitespace2(){
	const char *test_string = " \t \n \v \f \r abc123";
	test_value("abc123", jsonparser::parsing::delete_leading_white_space(test_string));
    printf("%s[%s:%d]:\n expected: %s\n actual: %s\n Test passed!\n",
        __FUNCTION__, __FILE__, __LINE__, "abc123", jsonparser::parsing::delete_leading_white_space(test_string));
}

void parse_whitespace3(){
    const char *test_string = " \t \n \v \f \r abc123";
    jsonparser::parsing::delete_leading_white_space(test_string);
    const char* expected = "abc123";
	test_value(expected, jsonparser::parsing::delete_leading_white_space(test_string));
    printf("%s[%s:%d]:\n expected: %s\n actual: %s\n Test passed!\n",
        __FUNCTION__, __FILE__, __LINE__, expected, jsonparser::parsing::delete_leading_white_space(test_string));
}

int main(){
    parse_string();
    parse_string2();
    reading_digits();
    reading_digits2();
    parse_number();
    parse_object();
    parse_empty_array();
    parse_string_array();
    parse_bool();
    parse_null();
    parse_whitespace();
    parse_whitespace2();
    parse_whitespace3();
}
