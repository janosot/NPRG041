# Developer's Documentation
Developer’s Documentation of the program JSON parser and serialiser.  
Programming in C++ (NPRG041) at MFF UK.  
Author: Tereza Janošová  
Date: 25.5.2022  
Version: 2.0

## Introduction
The JSON parser and serializer is a library that is situated into a header [jparser.hpp](jparser.hpp) file and source [jparser.cpp](jparser.cpp) file.   
This library can parse strings, arrays, objects, numbers, bools and null. It uses recursive descent parsing.

## Used tools
This library uses C++17. The main method for parsing is `parse(const std::string_view)`, which is a `std::variant` containing the types mentioned in the introduction section. All included libraries are standard libraries used from the C++17 version.  
To build the program, we use Makefile via `make` commands.

## Common Workflow
Common workflow is closely described in [README.md](README.md) file.

## Project Structure
The primary and only namespace is named `jsonparser`, which contains all library functionality.  
All types in JSON are parsed by calling `jsonparser::parse()` method. The method takes a string, detects its type and returns a parsed result.  
Calling the `jsonparser::serialize()` function will serialize the parsed result.

## Classes
Only two classes are used in this JSON parser library.  
A `ParsingError` class is used for catching and throwing exceptions from the parser functions.

The second class is a `Parser` class, which contains the grammar parsing functions.

## Decisions Made
Recursive descent parsing is used because it is a simple and straightforward way to parse JSON. Its grammar is LL(1). Only a single character lookahead is enough to make the grammar unambiguous.  
`std::variant` is used for type safety. In runtime, it allows us to find out what type it is without dynamic_cast.  
Custom exceptions are implemented to catch exceptions created during parsing.

## Encountered Pitfalls
I ran into a problem in defining the JSON type in the header. The fixed-point magic operator must be used so that the variant can contain a vector / map with itself.
