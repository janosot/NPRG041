#include "jparser.hpp"
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    // Parse and serialize json from input from parse.json file
    ifstream input("parse.json");
    string json;
    getline(input, json, '\0');
    jsonparser::jobject example = jsonparser::jobject::parse(json);
    cout << example.serialize() << endl;
}
