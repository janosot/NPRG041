#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <fstream>

#ifndef SHEEP_H_
#define SHEEP_H_

class Counter 
{
private:
    //Last char flags
    bool was_last_letter = false;
    bool was_last_alpha = false;
    bool was_last_num = false;

    //Number counting
    bool is_processing_number = false;
    long long unsigned int currnum = 0;
    long long unsigned int rownumcount = 0;

    //Word counting
    bool is_processing_word = false;
    long long unsigned int rowwordcount = 0;
    long long unsigned int sentencewordcount = 0;

    void process_word(char c);
    void process_number(char c);
    void process_sentence(char c);
    void process_row(char c);

public:
    long long unsigned int sum = 0;
    long long unsigned int numcount = 0;
    long long unsigned int wordcount = 0;
    long long unsigned int sentencecount = 0;
    long long unsigned int rowcount = 0;
    long long unsigned int charcount = 0;

    void process(std::istream& stream);
};

class IO 
{
private:
    void display(Counter c);
public:
    void run(std::vector<std::string>& args);
};

#endif