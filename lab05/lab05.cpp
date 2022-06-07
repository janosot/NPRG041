#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <iterator>

using namespace std;

void multiset_() 
{
    int x = 5;
    int y = -2;
    vector<int> vec{ 1,2,-4,10,9,-15,26,105,-7,0,6,11,-6,13 };
    multiset<int> set;

    vec.erase(remove_if(vec.begin(), vec.end(), [x](int i) {return i <= x; }), vec.end());

    transform(vec.begin(), vec.end(), inserter(set, set.end()), [y](int i) {return i + y; });

    for (auto&& i : set) 
    {
        cout << i << endl;
    }
}

class step_odcitac 
{
    int last;
    int diff;
public:
    bool operator() (int curr) 
    {
        bool result = (curr - last) > diff;
        last = curr;
        return result;
    }

    step_odcitac(int a, int b) 
    {
        last = a;
        diff = b;
    }

};

void step() 
{
    int n = 1;
    vector<int> vec{ 1,3,-4,10,9,-15,26,105,-7,0,6,11,-6,13 };

    auto res = find_if(vec.begin()++, vec.end(), step_odcitac(vec.front(), n));

    cout << *res << endl;
}

void increment() 
{
    int from = 5;
    int to = 10;
    int n = 2;
    int og_n = n;
    vector<int> vec{ 1,2,-4,10,9,-15,26,105,-7,0,6,11,-6,13 };
    for_each(vec.begin(), vec.end(), [from, to, &n, og_n](int& i) {if (from <= i && to >= i) { i += n; n += og_n; }});

    for (auto&& i : vec) 
    {
        cout << i << endl;
    }
}

class hole_odcitac 
{
    int last;
public:
    // -infinity
    int max_diff = 0x80000000;
    int max_val = 0x80000000;

    void operator()(const int& curr) 
    {
        if (curr - last > max_diff) 
        {
            max_diff = curr - last;
            max_val = curr;
        }
        last = curr;
    }
    hole_odcitac(int a) 
    {
        last = a;
    }
};

void hole() {
    vector<int> vec{ 1,2,-4,10,9,-15,26,105,-7,0,6,11,-6,13 };
    auto f = for_each(vec.begin()++, vec.end(), hole_odcitac(vec.front()));
    cout << f.max_val << endl;
}

void square() {
    vector<int> vec{ 1,2,-4,10,9,-15,26,105,-7,0,6,11,-6,13 };
    //vector<int> vec{1,2,5,3,4};
    vector<int> res;
    // ctyri prvky:
    //begin() half end()
    //  |     |     |
    //  v     v     v
    // [.][.][.][.]
    //pet
    //begin() half    end()
    //  |     |        |
    //  v     v        v
    // [.][.][.][.][.]
    transform(vec.begin(), vec.begin() + vec.size() / 2, vec.begin() + (vec.size() / 2 + (vec.size() % 2 ? 1 : 0)), back_inserter(res), [](int i, int j) {return i * i + j * j; });
    for (auto&& i : res) 
    {
        cout << i << endl;
    }
}



int main() 
{
    //multiset_();
    //step();
    //increment();
    //hole();
    //square();
    return 0;
}