#ifndef INIPARSER_H
#define INIPARSER_H

#include <fstream>
#include <boost/xpressive/xpressive.hpp>
//#include <regex>
#include "flags.h"
#include "utility.h"
#include "iniFile.h"

using namespace std;

class iniParser
{
    public:
        static iniFile load(string);
};

#endif // INIPARSER_H
