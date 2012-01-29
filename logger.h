#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

enum loggerFlags{
    E_NOTICE,
    E_WARNING,
    E_ERROR,
};

class logger
{
    public:
        static void setErrorReporting(loggerFlags = E_ERROR);
        static void setVerbose(bool = true);

        static bool open(string);
        static bool write(string, loggerFlags);
        static bool close();
    private:
        static fstream* fileHandler;
        static loggerFlags errorReporting;
        static bool verbose;
};

#endif // LOGGER_H
