#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <memory>
#include <string>

#include "seznam.hpp"

using namespace std;

int main() 
{
    Seznam list;
    list.add(make_unique<IntVal>(123));
    list.add(make_unique<StringVal>("abc"));
    Seznam druhy = list;
    druhy.add(make_unique<IntVal>(456));
    list.add(make_unique<StringVal>("cde"));

    cout << "list:" << endl;
    list.print();
    cout << endl << "druhy:" << endl;
    druhy.print();

    return 0;
}

