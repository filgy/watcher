#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class logger
{
    public:
        static logger* getInstance();
        bool open(string);
        bool write(string, bool = false);
        bool error(string, bool = false);
        bool close();

    private:
        logger(){};
        logger(logger const&){};
        //logger& operator =(logger const&){ };

        static logger* instance;
        static fstream* fileHandler;
};

#endif // LOGGER_H
