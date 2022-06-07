#include <istream>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "sheep.h"

using namespace std;

bool isletter(char c) 
{
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z');
}

bool isnumber(char c) 
{
    return (c >= '0' && c <= '9');
}

bool isalpha(char c) 
{
    return isnumber(c) || isletter(c);
}

//End of a sentence
bool isend(char c) 
{
    return (c == '.' || c == '!' || c == '?');
}

//Defining newline chars (because why would you have two?)
bool isnewline(char c) 
{
    return c == '\n';
}

void Counter::process_number(char c) 
{
    //Starting new number
    if (!was_last_alpha && isnumber(c)) 
    {
        is_processing_number = true;
        currnum = 0;
    }
    if (isnumber(c) && is_processing_number)
    {
        //append last digit
        currnum *= 10;
        currnum += c - '0';
    }
    //End of number
    else if (is_processing_number) 
    {
        is_processing_number = false;
        sum += currnum;
        currnum = 0;
        numcount++;
        rownumcount++;
    }
}

void Counter::process_word(char c) 
{
    //Starting new word
    if (!was_last_alpha && isletter(c)) 
    {
        is_processing_word = true;
    }
    //Ending word
    if (!isletter(c) && is_processing_word) 
    {
        is_processing_word = false;
        wordcount++;
        rowwordcount++;
        sentencewordcount++;
    }
}

void Counter::process_sentence(char c) 
{
    if (isend(c) && sentencewordcount != 0) 
    {
        sentencewordcount = 0;
        sentencecount++;
    }
}

void Counter::process_row(char c) 
{
    if (isnewline(c) && (rownumcount != 0 || rowwordcount != 0)) 
    {
        rownumcount = 0;
        rowwordcount = 0;
        rowcount++;
    }
}

void Counter::process(istream& stream) 
{
    //Initial values
    was_last_letter = false;
    was_last_alpha = false;
    was_last_num = false;
    is_processing_number = false;
    is_processing_word = false;
    rownumcount = 0;
    rowwordcount = 0;
    sentencewordcount = 0;

    char c;
    while (stream.get(c)) 
    {
        process_number(c);
        process_word(c);
        process_sentence(c);
        process_row(c);
        was_last_letter = isletter(c);
        was_last_alpha = isalpha(c);
        was_last_num = isnumber(c);
        charcount++;
    }
    //Last fake chars to trigger correct counts
    process_number('\0');
    process_word('\0');
    process_row('\n');

}

void IO::run(vector<string>& args) 
{
    Counter c;
    if (args.size() < 1)
        c.process(cin);
    else 
    {
        for (auto&& arg : args) 
        {
            if (arg == "-") 
            {
                c.process(cin);
            }
            else 
            {
                ifstream f;
                f.open(arg);
                if (f.good())
                    c.process(f);
            }
        }
    }
    display(c);
}

void IO::display(Counter c) 
{
    cout << "charc: " << c.charcount
        << " sum: " << c.sum
        << " numsc: " << c.numcount
        << " wordc: " << c.wordcount
        << " sentencec: " << c.sentencecount
        << " rowc: " << c.rowcount
        << endl;
}