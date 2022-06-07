# JSON parser & serializer - user documentation
JSON parsing and serialing library for C++, compatible with C++98

## Benefits
- Cross-platform (Linux, MacOS, Windows)
- No external dependencies

## Build 
Build JSON parser library via Makefile:
```
make
```
Simply `make` command will build [main.cpp](main.cpp), where is parsing from JSON file [parse.json](parse.json).  

Clean via Makefile:
```
make clean
```

## Tests
To test JSON parser functionality, run tests created in [test.cpp](test.cpp) via Makefile:

```
make tests
./test
```

## Usage
Usage of JSON parser and serializer is shown in [example.cpp](example.cpp) file.  
Run these examples via Makefile:
```
make examples
./example
```

``` cpp
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
```


