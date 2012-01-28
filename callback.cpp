#include "callback.h"

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

callback::callback(string file, string name, string logfile, unsigned int position, unsigned int size)
{
    this->file = file;
    this->name = name;
    this->logfile = logfile;
    this->position = position;
    this->size = size;
    this->triggers.clear();
}

void callback::setPosition(unsigned int position){
    this->position = position;
}

void callback::setSize(unsigned int size){
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

unsigned int callback::getPosition(){
    return this->position;
}

unsigned int callback::getSize(){
    return this->size;
}

int callback::getTriggersCount(){
    return this->triggers.size();
}

void callback::addTrigger(string name, string pattern, string command){
    trigger tmp(name, pattern, command);

    this->triggers.insert(make_pair(name, tmp));
}

map<string, trigger> callback::getTriggers(){
    return this->triggers;
}
