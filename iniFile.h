#ifndef INIFILE_H
#define INIFILE_H

#include <string>
#include <utility>
#include <list>
#include <map>

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

#endif // INIFILE_H
