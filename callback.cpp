#include "callback.h"

/*  - - - - - - - - - - - - - - - - - - - - - -
        Class: trigger
    ------------------------------------------- */
trigger::trigger(string name, string pattern, string command){
    this->name = name;
    this->pattern = pattern;
    this->command = command;
}

string trigger::getName(){
    return this->name;
}

string trigger::getPattern(){
    return this->pattern;
}

string trigger::getCommand(){
    return this->command;
}

/*  - - - - - - - - - - - - - - - - - - - - - -
        Class: callback
    ------------------------------------------- */

callback::callback(string file, string name, string logfile, long position, long size)
{
    this->file = file;
    this->name = name;
    this->logfile = logfile;
    this->position = position;
    this->size = size;
    this->triggers.clear();
}

void callback::setPosition(long position){
    this->position = position;
}

void callback::setSize(long size){
    this->size = size;
}

string callback::getFile(){
    return this->file;
}

string callback::getName(){
    return this->name;
}

string callback::getLogfile(){
    return this->logfile;
}

long callback::getPosition(){
    return this->position;
}

long callback::getSize(){
    return this->size;
}

int callback::getTriggersCount(){
    return this->triggers.size();
}

void callback::addTrigger(string name, string pattern, string command){
    this->triggers.insert(make_pair(name, trigger(name, pattern, command)));
}

map<string, trigger> callback::getTriggers(){
    return this->triggers;
}
