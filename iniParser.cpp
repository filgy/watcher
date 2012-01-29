#include "iniParser.h"

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
