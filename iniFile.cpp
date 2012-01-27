#include "iniFile.h"

iniFile::iniFile(){
    this->data.clear();
}

iniFile::~iniFile(){
    this->data.clear();
}

void iniFile::addSection(string sectionName){
    if(!this->isSection((sectionName)))
        this->data.insert(pair<string, map<string, string> >(sectionName, map<string, string>()));
}

bool iniFile::isSection(string sectionName){
     map<string, map<string, string> >::iterator p = this->data.find(sectionName);

     if(p == this->data.end())
        return false;
    else
        return true;
}

void iniFile::setValue(string sectionName, string directive, string value){
    if(!this->isSection(sectionName))
        return;

    if(this->data[sectionName].find(directive) == this->data[sectionName].end())
        this->data[sectionName].insert(pair<string, string>(directive, value));
    else
        this->data[sectionName][directive] = value;
}

string iniFile::getValue(string sectionName, string directive){
    if(!this->isSection(sectionName))
        return "";
    else if(this->data[sectionName].find(directive) == this->data[sectionName].end())
        return "";
    else
        return this->data[sectionName][directive];
}

list<string> iniFile::getSections(){
    list<string> sections;

    for(map<string, map<string, string> >::iterator p = this->data.begin(); p != this->data.end(); p++)
        sections.push_back((*p).first);

    return sections;
}
