#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "film.h"

using namespace std;

int main(int argc, char** argv) 
{
    film a("Harry Potter and the Philosopher's Stone", "Chris Columbus", 2001);
    film b("Harry Potter and the Chamber of Secrets", "Chris Columbus", 2002);
    film c("Fantastic Beasts and Where to Find Them", "Davis Yates", 2016);
    film d("Hacksaw Ridge", "Mel Gibson", 2016);
    film e("Shrek", "Vicky Jenson", 2001);
    film f("Iron Man", "Jon Favreau", 2008);
    std::vector<film> vec{ a,b,c,d,e,f };

    std::sort(vec.begin(), vec.end());

    for (auto i : vec)
        std::cout << i.name << " " << i.director << " " << i.year << std::endl;
}
