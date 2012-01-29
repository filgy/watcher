#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class utility
{
    public:
        static string replace(string, string, string);
        static string trim(string);
        static string ltos(long);
        static string itos(int);
        static long stol(string);
        static int stoi(string);
        static long fileSize(string);
};

#endif // UTILITY_H
