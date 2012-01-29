#include "logger.h"

fstream* logger::fileHandler = NULL;

loggerFlags logger::errorReporting = E_ERROR;

bool logger::verbose = false;

void logger::setErrorReporting(loggerFlags flag){
    logger::errorReporting = flag;
}

void logger::setVerbose(bool verbose){
    logger::verbose = verbose;
}

bool logger::open(string fileName){
    logger::fileHandler = new fstream(fileName.c_str(), ios::out);

    if(logger::fileHandler->is_open())
        return true;

    delete logger::fileHandler;
    return false;
}

bool logger::write(string message, loggerFlags flag){
    if(flag < logger::errorReporting)
        return false;

    message = ((flag >= E_WARNING)? "[!] " : "[*] ") + message;

    if(logger::verbose)
        cout << message << endl;

    if(logger::fileHandler && logger::fileHandler->is_open()){
        *(logger::fileHandler) << message << endl;
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


