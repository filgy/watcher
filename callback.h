#ifndef CALLBACK_H
#define CALLBACK_H

#include <string>
#include <map>

using namespace std;

class trigger
{
    public:
        trigger(string, string, string);

        string getName();
        string getPattern();
        string getCommand();

    private:
        string name;
        string pattern;
        string command;
};

class callback
{
    public:
        callback(string, string, unsigned int = 0, unsigned int = 0);

        void setPosition(unsigned int);
        void setSize(unsigned int);

        string getName();
        string getLogfile();
        unsigned int getPosition();
        unsigned int getSize();
        int getTriggersCount();

        void addTrigger(string, string, string);

        map<string, trigger> getTriggers();
    protected:
    private:
        string name;
        string logfile;
        unsigned int position;
        unsigned int size;

        map<string, trigger> triggers;

};



#endif // CALLBACK_H
