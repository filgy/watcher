#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <sstream>

using namespace std;

class utility
{
    public:
        static string itos(int);
        static int stoi(string);
    private:
        utility();
};

#endif // UTILITY_H
