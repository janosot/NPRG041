#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>

#include "dictionary.h"

using namespace std;
using vs = vector<string>;

int dictionary::remove_all(const string& word) 
{
    auto res = data.find(word);
    if (res == data.cend()) 
    {
        return 1;
    }
    else {
        data.erase(res);
    }
    return 0;
}
int dictionary::add(const string& word, const string& translation) 
{
    auto res = data.find(word);
    if (res == data.cend()) 
    {
        vs to_add(1, translation);
        data.emplace(make_pair(word, to_add));
    }
    else 
    {
        res->second.push_back(translation);
    }
    return 0;
}
int dictionary::remove(const string& word, const string& translation) 
{
    auto res = data.find(word);
    if (res == data.cend()) 
    {
        return 1;
    }
    else 
    {
        auto iter = res->second.cbegin();
        for (; iter != res->second.cend() && *iter != translation; ++iter) {}

        if (iter == res->second.cend()) 
        {
            return 2;
        }
        else {
            res->second.erase(iter);
        }
    }
    return 0;
}
vs dictionary::find(const string& word) const 
{
    auto res = data.find(word);
    if (res == data.cend()) 
    {
        cerr << "Key: " << word << " not found while trying to find " << word << endl;
        return vs();
    }
    else 
    {
        vs result(res->second);
        sort(result.begin(), result.end(), [](string a, string b) {return a.size() < b.size(); });
        return result;
    }
}
vs dictionary::pfind(const string& prefix) const 
{
    vs result;
    for (auto&& i : data) 
    {
        size_t k = 0;
        for (; k < i.first.size() && k < prefix.size() && prefix[k] == i.first[k]; ++k);

        if (k == prefix.size()) {
            result.push_back(i.first);
        }
    }

    sort(result.begin(), result.end(), [](string a, string b) {return a.size() < b.size(); });

    return result;
}
vs dictionary::pafind(const string& prefix) const 
{
    vs result;
    for (auto&& i : data)
    {
        for (auto&& j : i.second)
        {
            size_t k = 0;
            for (; k < j.size() && k < prefix.size() && prefix[k] == j[k]; ++k);

            if (k == prefix.size()) 
            {
                result.push_back(j);
            }
        }
    }

    sort(result.begin(), result.end(), [](string a, string b) {return a.size() < b.size(); });

    return result;
}

vs dictionary::rfind(const string& translation) const 
{
    vs result;
    for (auto&& i : data) 
    {
        for (auto&& j : i.second) 
        {
            if (j == translation) 
            {
                result.push_back(i.first);
                continue;
            }
        }
    }
    if (!result.size()) {
        cerr << "Reverse translation of: " << translation << " not found!" << endl;
    }

    sort(result.begin(), result.end(), [](string a, string b) {return a.size() < b.size(); });

    return result;
}


bool dictionary::has(const string& word) const 
{
    return data.find(word) != data.cend();
}
