#ifndef WATCHDOG_H
#define WATCHDOG_H

//#include <boost/xpressive/xpressive.hpp>
#include "flags.h"
#include "iniFile.h"
#include "iniParser.h"
#include "callback.h"
#include <sys/types.h>
#include <dirent.h>
#include <list>
#include <map>

using namespace std;

class watchdog
{
    public:
        watchdog(string callbackDir);
        virtual ~watchdog();

        list<string> findCallbacks();
        void loadCallbacks();


        void DEBUG();
    protected:
    private:
        string callbackDir;

        map<string, callback> suspects;
};

#endif // WATCHDOG_H
