#pragma once
#include <string>
struct film 
{
    std::string name;
    std::string director;
    int year;
    film() :name(""), director(""), year(0) {}
    film(std::string _name, std::string _director, int _year)
    {
        name = _name;
        director = _director;
        year = _year;
    }
    bool operator<(film other) 
    {
        return year == other.year ? name < other.name : year < other.year;
    }
    bool operator==(film other) 
    {
        return year == other.year && name == other.name;
    }
    bool operator!=(film other) 
    {
        return !(*this == other);
    }
    bool operator<=(film other) 
    {
        return *this < other || *this == other;
    }
    bool operator>(film other) 
    {
        return !(*this <= other);
    }
    bool operator>=(film other) 
    {
        return !(*this < other);
    }
};
