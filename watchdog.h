#ifndef WATCHDOG_H
#define WATCHDOG_H

//#include <boost/xpressive/xpressive.hpp>
#include "logger.h"
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
        watchdog(string callbackDir, unsigned int = 1);
        virtual ~watchdog();

        list<string> findCallbacks();

        void loadCallbacks();
        void saveCallbacks();

        void run();

        void DEBUG();
    private:
        string callbackDir;
        unsigned int interval;

        map<string, callback> suspects;

        string itos(int);
        int stoi(string);
};

#endif // WATCHDOG_H
