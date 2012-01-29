#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class utility
{
    public:
        //String
        static string replace(string, string, string);
        static string trim(string);

        //Conversion
        static string ltos(long);
        static string itos(int);
        static long stol(string);
        static int stoi(string);

        //Files
        static long fileSize(string);
};

#endif // UTILITY_H
