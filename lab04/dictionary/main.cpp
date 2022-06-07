#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "dictionary.h"

using namespace std;
void test() {
    dictionary cz_en_dict;
    cz_en_dict.add("psoblb", "dogtard");
    cz_en_dict.add("pes", "dog");

    cz_en_dict.add("prace", "work");
    cz_en_dict.add("dilo", "work");
    cz_en_dict.add("kocka", "cat");
    cz_en_dict.add("kocka", "kitty");


    cz_en_dict.add("pes", "doggo");
    cz_en_dict.add("pes", "doggie");
    cz_en_dict.add("pes", "doggy23");
    cz_en_dict.add("pes", "doggy156");
    cz_en_dict.add("pes", "doggy19819");
    cz_en_dict.add("pes", "doggy98498");
    cz_en_dict.add("pes", "doggy0");

    if (cz_en_dict.remove_all("pes")) {
        //Chyba
        cerr << "test" << endl;
    }

    for (auto&& word : cz_en_dict.find("kocka")) {
        cout << word << endl;
    }
    cout << endl;
    for (auto&& word : cz_en_dict.pfind("dog")) {
        cout << word << endl;
    }
    cout << endl;
    for (auto&& word : cz_en_dict.rfind("work"))
        cout << word << endl;
}

int process() 
{
    dictionary dict;

    while (cin.good()) 
    {
        string command;
        cin >> command;

        if (command == "add") 
        {
            string word, trans;
            cin >> word;
            cin >> trans;
            if (dict.add(word, trans))
                cout << "Something went wrong!" << endl;
        }
        else if (command == "remove") 
        {
            string word, trans;
            cin >> word;
            cin >> trans;
            if (dict.remove(word, trans))
                cout << "Something went wrong!" << endl;
        }
        else if (command == "removeall") 
        {
            string word;
            cin >> word;
            if (dict.remove_all(word))
                cout << "Something went wrong!" << endl;
        }
        else if (command == "find")
        {
            string word;
            cin >> word;
            for (auto&& i : dict.find(word)) {
                cout << i << endl;
            }
        }
        else if (command == "has") 
        {
            string word;
            cin >> word;
            cout << dict.has(word) << endl;
        }
        else if (command == "pfind") 
        {
            string prefix;
            cin >> prefix;
            for (auto&& i : dict.pfind(prefix)) 
            {
                cout << i << "\t";
                for (auto&& j : dict.find(i))
                    cout << j << " ";
                cout << endl;
            }
        }
        else if (command == "pafind")
        {
            string prefix;
            cin >> prefix;
            for (auto&& i : dict.pafind(prefix))
            {
                cout << i << endl;
            }
        }
        else if (command == "rfind") 
        {
            string word;
            cin >> word;
            for (auto&& i : dict.rfind(word)) 
            {
                cout << i << endl;
            }
        }
        else if (cin.good()) 
        {
            cout << "Wrong command!" << endl;
        }
    }
    return 0;
}

int main(int argc, char** argv) 
{
    return process();
}
