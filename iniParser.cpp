#include "iniParser.h"

/*  - - - - - - - - - - - - - - - - - - - - - -
        Class: iniFile
    ------------------------------------------- */
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

     return (p == this->data.end())? false : true;
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

    return (this->data[sectionName].find(directive) == this->data[sectionName].end())? "" : this->data[sectionName][directive];
}

list<string> iniFile::getSections(){
    list<string> sections;

    for(map<string, map<string, string> >::iterator p = this->data.begin(); p != this->data.end(); p++)
        sections.push_back((*p).first);

    return sections;
}

/*  - - - - - - - - - - - - - - - - - - - - - -
        Class: iniParser
    ------------------------------------------- */
iniFile iniParser::load(string fileName){
    fstream fileHandler(fileName.c_str(), ios::in);

    if(!fileHandler.is_open())
        throw PARSER_CANNOT_OPEN_FILE;

    iniFile file;

    string line;
    string section = "General";
    string section_regul = "^\\[([A-z0-9\\-]+)\\]$";
    string value_regul = "^([^\\=]+)\\=(.+)$";

    regexApiMatch match;

    while(getline(fileHandler, line)){
        line = utility::trim(line);

        if(regexApi::preg_match(section_regul, line, match)){
            section = match[1];

            file.addSection(section);
        }
        else if(regexApi::preg_match(value_regul, line, match)){
            if(!file.isSection(section))
                file.addSection(section);

            file.setValue(section, match[1], match[2]);
        }
        else{
            //piece of shit
        }
    }

    fileHandler.close();

    return file;
}
