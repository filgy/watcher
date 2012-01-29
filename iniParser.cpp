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
        line = utility::trim(line);

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
