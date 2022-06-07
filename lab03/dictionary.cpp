#include <vector>
#include <string>
#include <map>
#include <iostream>

#include "dictionary.h"

using namespace std;
using vs = vector<string>;

void dictionary::remove_all(const string& word) 
{
    auto res = data.find(word);
    if (res == data.cend()) 
    {
        cerr << "Trying to remove nonexistent word: " << word << endl;
    }
    else 
    {
        data.erase(res);
    }
}

void dictionary::add(const string& word, const string& translation) 
{
    auto res = data.find(word);
    if (res == data.cend()) 
    {
        vs to_add(1, translation);
        data.emplace(make_pair(word, to_add));
    }
    else {
        res->second.push_back(translation);
    }
}

void dictionary::remove(const string& word, const string& translation) 
{
    auto res = data.find(word);
    if (res == data.cend()) 
    {
        cerr << "Key: " << word << " not found! Trying to remove " << translation << endl;
    }
    else 
    {
        auto iter = res->second.cbegin();
        for (; iter != res->second.cend() && *iter != translation; ++iter) {}
        if (iter == res->second.cend()) 
        {
            cerr << "Trying to remove nonexistent translation: " << translation << " not found for word: " << word << endl;
        }
        else
        {
            res->second.erase(iter);
        }
    }
}

vs dictionary::find(const string& word) 
{
    auto res = data.find(word);
    if (res == data.cend())
    {
        cerr << "Key: " << word << " not found while trying to find " << endl;
        return vs();
    }
    else 
    {
        return res->second;
    }
}

bool dictionary::has(const string& word) 
{
    auto res = data.find(word);
    return res != data.cend();
}