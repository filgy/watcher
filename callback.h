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
        callback(string, string, string, long = 0, long = 0);

        void setPosition(long);
        void setSize(long);

        string getFile();
        string getName();
        string getLogfile();
        long getPosition();
        long getSize();
        int getTriggersCount();

        void addTrigger(string, string, string);

        map<string, trigger> getTriggers();
    protected:
    private:
        string file;
        string name;
        string logfile;
        long position;
        long size;

        map<string, trigger> triggers;

};



#endif // CALLBACK_H
