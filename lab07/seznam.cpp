#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include "seznam.hpp"

using namespace std;

void Seznam::add(std::unique_ptr<AbstractVal>&& p)
{
    pole_.push_back(move(p));
}

void Seznam::print()
{
    for (auto&& i : this->pole_)
    {
        i->print();
        cout << endl;
    }
}

Seznam& Seznam::operator=(const Seznam& other)
{
    if (this == &other)
    {
        return *this;
    }
    pole_.clear();
    clone(other);
    return *this;
}

Seznam::Seznam(const Seznam& other)
{
    clone(other);
}
