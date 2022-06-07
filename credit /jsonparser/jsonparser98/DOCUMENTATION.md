# JSON parser & serializer - developer's documentation

## Structure
The JSON parser is a library that is situated into a header [jparser.hpp](jparser.hpp) file and source [jparser.cpp](jparser.cpp) file.  

The base namespace for this library is named `jsonparser`. 
Objects in JSON are parsed by calling `jsonparser::jobject::parse()` method. The method takes a string and returns `jobject`. 

Entries can be accessed and assigned by using the `[]` operators.  

The instance `jobject` can be serialized using `serialize()` method. However this instance does not retain original formatting - tabs, spaces, outside strings and newlines are deleted. 

Arrays are supported as a root objects, which is shown in [example.cpp](example.cpp) file. They are also stored in `jobject` and are determinated using `is_array()` method.
``` cpp
const char* input = "[1, 2, 3]";
jsonparser::jobject example = jsonparser::jobject::parse(input);
// "a" value is 1
int a = example[0]; 
// "b" value is 2
int b = example[1];
// "c" value is 3
int c = example[2];
```
Arrays can also access multi-level using the `get(key)` and `array(index)`, which is also shown in [example.cpp](example.cpp) file. 
``` cpp 
std::string beholdings = example.array(0).get("holding").array(0).get("car").as_string() 
        + ", " + example.array(0).get("holding").array(1).get("phone").as_string();
```