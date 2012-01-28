#include "utility.h"

utility::utility()
{
    //ctor
}

string utility::itos(int number){
    ostringstream str;
    str << number;
    return str.str();
}

int utility::stoi(string str){
    istringstream is(str);
    int number;
    is >> number;
    return number;
}

