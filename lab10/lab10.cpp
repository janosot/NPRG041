#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <random>
#include <ctime>
#include <functional>
#include <chrono>

#include "guma.hpp"

using namespace std;


void test1()
{
    Pole<int> p(2);
    p.push_back(1);
    p.push_back(2);
    p.push_back(3);
    p.push_back(4);

    vector<int> vec{ 1,2,3,4 };

    auto it = mismatch(vec.begin(), vec.end(), p.begin(), p.end());

    if (it.first != vec.end() || it.second != p.end())
    {
        cerr << "Test 1 failed!" << endl;
        return;
    }

    cerr << "Test 1 succedded!" << endl;

}
void test2()
{
    // copy ctor
    Pole<int> p(10);

    p.push_back(5);
    p.push_back(6);
    p.push_back(7);
    p.push_back(-5);
    p.push_back(51);
    p.push_back(25);
    p.push_back(-55);
    p.push_back(155);
    p.push_back(5165);
    p.push_back(5651);
    p.push_back(42);
    p.push_back(7);
    p.push_back(65);
    p.push_back(95);
    Pole<int> q(p);

    auto it = mismatch(p.begin(), p.end(), q.begin(), q.end());
    if (it.first != p.end() || it.second != q.end())
    {
        cerr << "Test 2 failed!" << endl;
        return;
    }
    cerr << "Test 2 succedded!" << endl;
}
void test3()
{
    // move ctor
    Pole<int> p(7);

    p.push_back(1);
    p.push_back(2);
    p.push_back(3);
    p.push_back(4);

    Pole<int> q(move(p));

    vector<int> vec{ 1,2,3,4 };

    auto it = mismatch(vec.begin(), vec.end(), q.begin(), q.end());

    if (it.first != vec.end() || it.second != q.end())
    {
        cerr << "Test 3 failed!" << endl;
        return;
    }

    cerr << "Test 3 succedded!" << endl;
}
void test4()
{
    //copy assign
    Pole<int> p(3);

    p.push_back(5);
    p.push_back(6);
    p.push_back(7);
    p.push_back(-5);
    p.push_back(51);
    p.push_back(25);
    p.push_back(-55);
    p.push_back(155);
    p.push_back(5165);
    p.push_back(5651);
    p.push_back(42);
    p.push_back(7);
    p.push_back(65);
    p.push_back(95);
    Pole<int> q(100);
    q = p;

    auto it = mismatch(p.begin(), p.end(), q.begin(), q.end());
    if (it.first != p.end() || it.second != q.end())
    {
        cerr << "Test 4 failed!" << endl;
        return;
    }
    cerr << "Test 4 succedded!" << endl;
}
void test5()
{
    //move assign
    Pole<int> p(6);

    p.push_back(1);
    p.push_back(2);
    p.push_back(3);
    p.push_back(4);

    Pole<int> q(21);
    q = move(p);

    vector<int> vec{ 1,2,3,4 };

    auto it = mismatch(vec.begin(), vec.end(), q.begin(), q.end());

    if (it.first != vec.end() || it.second != q.end())
    {
        cerr << "Test 5 failed!" << endl;
        return;
    }

    cerr << "Test 5 succedded!" << endl;
}
void test6()
{
    Pole<int> p(2);
    p.push_back(1);
    p.push_back(2);
    p.push_back(3);
    p.push_back(4);

    vector<int> vec{ 1,2,3,4 };

    for (size_t i = 0; i < 4; ++i)
    {
        if (vec[i] != p[i])
        {
            cerr << "Test 6 failed!" << endl;
            return;
        }
    }
    cerr << "Test 6 succedded!" << endl;

}
void test7()
{
    //operator *
    Pole<int> p(2);
    p.push_back(1);
    p.push_back(2);
    p.push_back(3);
    p.push_back(4);
    auto it = p.begin();
    if (*it != 1)
    {
        cerr << "Test 7 failed!" << endl;
        return;
    }
    cerr << "Test 7 succedded!" << endl;
}
void test8()
{
    //operator != 
    Pole<int> p(2);
    if (!(p.begin() != p.end()))
    {
        cerr << "Test 8 failed!" << endl;
        return;
    }
    cerr << "Test 8 succedded!" << endl;
}
void test9()
{
    //operator ++
    Pole<int> p(2);
    p.push_back(1);
    p.push_back(2);
    p.push_back(3);
    p.push_back(4);

    vector<int> vec{ 1,2,3,4 };
    size_t i = 0;
    for (auto it = p.begin(); it != p.end(); ++it)
    {
        if (vec[i++] != (*it))
        {
            cerr << "Test 9 failed!" << endl;
            return;
        }
    }
    cerr << "Test 9 succedded!" << endl;
}
void test10()
{
    //edge case
    Pole<int> p(6);
    p.push_back(1);
    p.push_back(2);
    p.push_back(3);
    p.push_back(4);

    auto it = p.begin();

    ++it; ++it; ++it;

    for (size_t i = 0; i < 1000; ++i)
    {
        p.push_back(i + 5);
    }

    auto it2 = p.begin();
    ++it2; ++it2; ++it2;

    if (it != it2)
    {
        cerr << "Test 10 failed!" << endl;
        return;
    }
    cerr << "Test 10 succedded!" << endl;
}

void test11() 
{
    Pole<int> p(6);
    p.push_back(1);
    p.push_back(2);
    p.push_back(3);
    p.push_back(4);
    try 
    {
        cout << p.at(5) << endl;
    }
    catch (myexc & e) 
    {
        cerr << "Test 11 succedded!" << endl;
        return;
    }
    catch (exception & e) 
    {
        cerr << "Unexpected exception!" << endl;
    }
    cerr << "Test 11 failed!" << endl;
}
void test12() 
{
    Pole<int> p(6);
    p.push_back(1);
    p.push_back(2);
    p.push_back(3);
    p.push_back(4);

    try 
    {
        auto it = p.end();
        *end(p);
    }
    catch (out_of_range & e)
    {
        cerr << "Test 12 succedded!" << endl;
        return;
    }
    catch (exception & e) 
    {
    }
    cerr << "Test 12 failed!" << endl;

}
void test13() {
    Pole<int> p(6);
    p.push_back(1);
    p.push_back(2);
    p.push_back(3);
    p.push_back(4);
    try {
        ++end(p);
    }
    catch (out_of_range & e)
    {
        cerr << "Test 13 succedded!" << endl;
        return;
    }
    catch (exception & e) 
    {
    }
    cerr << "Test 13 failed!" << endl;

}

void strelba() {
    int count = 9985;
    int chunk = 100;

    Pole<int> p(chunk);
    for (size_t i = 0; i < count; i++)
    {
        p.push_back(i);
    }

    uniform_int_distribution<int> distrib{ 0,(count / chunk + 1) * (chunk) };
    default_random_engine generator(time(0));

    auto dice = bind(distrib, generator);

    //for some reason first one is always the same
    dice();

    size_t i = 0;
    try {
        for (;;) {
            p.at(dice());
            i++;
        }
    }
    catch (myexc & e) {
        cerr << e.what() << endl;
        cerr << "It took: " << i << " attempts" << endl;
    }
}
int main() 
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    test10();
    test11();
    test12();
    test13();
    strelba();
   
}
