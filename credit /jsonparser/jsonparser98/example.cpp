#include "jparser.hpp"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

// Examples of usage
void parse_object_array() {
    // create some json objects
    jsonparser::jobject a = jsonparser::jobject::parse("{\"hello\":\"world\"}");
    jsonparser::jobject b;
    b["hi"] = "another world";
    // create an array of objects
    vector<jsonparser::jobject> objects;
    objects.push_back(a);
    objects.push_back(b);
    // create a json object with the array
    jsonparser::jobject example;
    example["array"] = objects;
    // print the result
    printf("%s\n", example.serialize().c_str());
    // access each item
    assert(example["array"].array(0) == a);
    assert(example["array"].array(1) == b);
    // read back the entire array
    vector<jsonparser::jobject> array = example["array"];
    // access items
    jsonparser::jobject readback1 = array[0];
    assert(readback1 == a);
    jsonparser::jobject readback2 = array[1];
    assert(readback2 == b);
}

void parse_root_array() {
    // parse the objects
    jsonparser::jobject example = jsonparser::jobject::parse
        ("[{\"firstName\":\"Tess\",\"lastName\":\"J\",\"holding\":[{\"car\":\"Kia Ceed\"},{\"phone\":\"iPhone\"}]}]");
    // access the first object
    string beholdings = example.array(0).get("holding").array(0).get("car").as_string() 
        + ", " + example.array(0).get("holding").array(1).get("phone").as_string();
    // print the result
    printf("Tess ownes: %s\n", beholdings.c_str());
    // check the result
    assert(beholdings == string("Kia Ceed, iPhone"));
}

int main() {
    parse_object_array();
    parse_root_array();
}
