#include "utility.h"

utility::utility()
{
    //ctor
}

string utility::ltos(long number){
    ostringstream str;
    str << number;
    return str.str();
}

long utility::stol(string str){
    istringstream is(str);
    long number;
    is >> number;
    return number;
}

long utility::fileSize(string fileName){
    ifstream fileHandler(fileName.c_str());

    if(!fileHandler.is_open())
        return 0;

    fileHandler.seekg(0, ios::end);

    long begin = 0;
    long end = fileHandler.tellg();

    fileHandler.close();

    return (end - begin);
}

