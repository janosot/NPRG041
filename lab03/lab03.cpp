#include <string>
#include <vector>
#include <iostream>

#include "dictionary.h"

using namespace std;

int main(int argc, char** argv) 
{
    dictionary cz_en_dict;
    cz_en_dict.add("pes", "dog");
    cz_en_dict.add("pes", "doggy");
    cz_en_dict.add("prace", "work");
    cz_en_dict.add("kocka", "cat");
    cz_en_dict.add("kocka", "kitty");
    cz_en_dict.remove_all("pes");
    for (auto&& word : cz_en_dict.find("kocka")) 
    {
        cout << word << endl;
    }
    for (auto&& word : cz_en_dict.find("kun"))
    {
        cout << word << endl;
    }
}