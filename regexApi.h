#ifndef REGEXAPI_H
#define REGEXAPI_H

#include <boost/xpressive/xpressive.hpp>
#include <string>
#include <vector>

using namespace std;

class regexApiMatch{

    public:
        regexApiMatch();
        regexApiMatch(vector<string>);

        int size();
        void clear();
        void insert(string);
        string operator[](unsigned int);
    private:
        vector<string> matches;
};

class regexApi
{
    public:
        static bool preg_match(string, string);
        static bool preg_match(string, string, regexApiMatch&);

};

#endif // REGEXAPI_H
