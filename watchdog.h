#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <list>
#include <map>

#include "flags.h"
#include "logger.h"
#include "regexApi.h"
#include "iniParser.h"
#include "callback.h"
#include "utility.h"

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

        static void getsig(int);
    private:
        string callbackDir;
        unsigned int interval;

        map<string, callback> suspects;

        static bool active;
};

#endif // WATCHDOG_H
