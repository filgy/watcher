#include "iniParser.h"

iniFile iniParser::load(string fileName){
    fstream fileHandler(fileName.c_str(), ios::in);

    if(!fileHandler.is_open())
        throw PARSER_CANNOT_OPEN_FILE;

    iniFile file;

    string line;
    string section = "General";

    boost::xpressive::sregex section_regul = boost::xpressive::sregex::compile("^\\[([A-z0-9\\-]+)\\]$");
    boost::xpressive::sregex value_regul = boost::xpressive::sregex::compile("^([^\\=]+)\\=(.+)$");
    boost::xpressive::smatch match;

    //regex section_regul("^\\[([A-z0-9]+)\\]$", regex_constants::extended);
    //regex value_regul("^([^\\=]+)\\=(.+)$", regex_constants::extended);
    //smatch match;

    while(getline(fileHandler, line)){
        line = iniParser::trim(line);

        if(boost::xpressive::regex_match(line, match, section_regul)){
        //if(regex_match(line, match, section_regul)){
            section = match[1];

            file.addSection(section);
        }
        else if(boost::xpressive::regex_match(line, match, value_regul)){
        //else if(regex_match(line, match, value_regul)){
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

string& iniParser::trim(string& str){

    unsigned int start,end;

    for(start = 0; start < str.length() && str.at(start) == ' '; start++);

    for(end = str.length(); str.length() > 0 && end > 0 && str.at(end - 1) == ' '; end--);

    str = str.substr(start, end - start);

    return str;
}
