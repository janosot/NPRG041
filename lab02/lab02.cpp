#include <vector>
#include <string>
#include "sheep.h"

using namespace std;

int main(int argc, char** argv) 
{
    vector<string> arg(argv + 1, argv + argc);
    IO a;
    a.run(arg);
}