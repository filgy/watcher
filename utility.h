#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class utility
{
    public:
        static string ltos(long);
        static long stol(string);
        static long fileSize(string);
    private:
        utility();
};

#endif // UTILITY_H
