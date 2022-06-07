#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <iterator>

#include "guma.hpp"

using namespace std;

int main()
{
    Pole<int> p(2);

    p.push_back(1);
    p.push_back(2);
    p.push_back(3);
    p.push_back(4);

    Pole<int> q(move(p));

    for (auto&& i : q)
        cout << i << endl;
}