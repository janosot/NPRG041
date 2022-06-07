#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <memory>

#include "scitacka.hpp"
#include "seznam.hpp"

using namespace std;

int main() 
{
    Seznam list;
    list.add(make_unique<ConcreteVal<fraction<int>>>(fraction<int>(1, 2)));
    list.add(make_unique<ConcreteVal<fraction<int>>>(fraction<int>(1, 3)));
    Seznam druhy = list;
    druhy.add(make_unique<ConcreteVal<fraction<float>>>(fraction<float>(1.1, 2.2)));
    list.add(make_unique<ConcreteVal<fraction<float>>>(fraction<float>(0.3, 2.5)));

    cout << "list:" << endl;
    list.print();
    cout << endl << "druhy:" << endl;
    druhy.print();


    /*
    S<fraction<int>> s,t;
    s.add(fraction<int>(1,2));
    t.add(fraction<int>(1,4));
    t.add(fraction<int>(1,8));
    s=s+t;

    cout<<s.result().print()<<endl;


    fraction<float> a(1,2);
    fraction<float> b(1.1,2.0);
    b+=a;
    cout<<b.print()<<endl;
    */

    return 0;
}
