#include "logger.h"

logger* logger::instance = NULL;
fstream* logger::fileHandler = NULL;

logger* logger::getInstance(){
    if(!instance)
        instance = new logger;

    return instance;
}

bool logger::open(string fileName){
    logger::fileHandler = new fstream(fileName.c_str(), ios::out);

    if(logger::fileHandler->is_open())
        return true;
    else{
        delete logger::fileHandler;
        return false;
    }
}

bool logger::write(string message, bool verbose){
    if(verbose)
        cout << "[*] " << message << endl;

    if(logger::fileHandler && logger::fileHandler->is_open()){
        *(logger::fileHandler) << "[*] " << message << endl;
        return true;
    }

    return false;
}

bool logger::error(string message, bool verbose){
    if(verbose)
        cerr << "[!] " << message << endl;

    if(logger::fileHandler && logger::fileHandler->is_open()){
        *(logger::fileHandler) << "[!] " << message << endl;
        return true;
    }

    return false;
}

bool logger::close(){
    if(logger::fileHandler && logger::fileHandler->is_open()){
        logger::fileHandler->close();
        delete logger::fileHandler;
        return true;
    }

    return false;
}


