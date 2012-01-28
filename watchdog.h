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
        watchdog(string callbackDir);
        virtual ~watchdog();

        list<string> findCallbacks();

        void loadCallbacks();
        void saveCallbacks();

        void DEBUG();
    protected:
    private:
        string callbackDir;

        map<string, callback> suspects;

        string itos(int);
};

#endif // WATCHDOG_H
