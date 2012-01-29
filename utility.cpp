#include "utility.h"

/*  - - - - - - - - - - - - - - - - - - - - - -
        Utilities for strings
    ------------------------------------------- */
string utility::replace(string search, string replace, string subject){
    size_t pos;

    while((pos = subject.find(search)) != string::npos)
        subject = subject.replace(pos, search.length(), replace);

    return subject;
}

string utility::trim(string subject){
    size_t start,end;

    for(start = 0; start < subject.size() && subject.at(start) == ' '; start++);
    for(end = subject.size(); subject.size() > 0 && end > 0 && subject.at(end - 1) == ' '; end--);

    return subject.substr(start, end - start);
}

/*  - - - - - - - - - - - - - - - - - - - - - -
        Utilities for conversion
    ------------------------------------------- */
string utility::ltos(long number){
    ostringstream stream;
    stream << number;
    return stream.str();
}

string utility::itos(int number){
    ostringstream stream;
    stream << number;
    return stream.str();
}

long utility::stol(string str){
    istringstream stream(str);
    long number;
    stream >> number;
    return number;
}

int utility::stoi(string str){
    istringstream stream(str);
    int number;
    stream >> number;
    return number;
}

/*  - - - - - - - - - - - - - - - - - - - - - -
        Utilities for files
    ------------------------------------------- */
long utility::fileSize(string filename){
    long position;
    ifstream fileHandler(filename.c_str());

    if(!fileHandler.is_open())
        return 0;

    fileHandler.seekg(0, ios::end);

    position = fileHandler.tellg();

    fileHandler.close();

    return position;
}

