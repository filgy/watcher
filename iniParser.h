#ifndef INIPARSER_H
#define INIPARSER_H

#include <fstream>
#include <string>
#include <utility>
#include <list>
#include <map>

#include "flags.h"
#include "regexApi.h"
#include "utility.h"

using namespace std;

class iniFile
{
    public:
        iniFile();
        virtual ~iniFile();

        void addSection(string);
        bool isSection(string);

        void setValue(string, string, string);
        string getValue(string, string);

        list<string> getSections();

    private:
        map<string, map<string, string> > data;
};

class iniParser
{
    public:
        static iniFile load(string);
};

#endif // INIPARSER_H
