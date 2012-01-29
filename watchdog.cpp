#include "watchdog.h"

bool watchdog::active = true;

watchdog::watchdog(string callbackDir, unsigned int interval){
    signal(SIGINT, watchdog::getsig);

    this->callbackDir = callbackDir;
    this->interval = interval;

    //Setup logger
    logger::setVerbose(true);
    logger::setErrorReporting(E_NOTICE);
    logger::open("./server.log");

    try{
        logger::write("Watchdog starting...", E_NOTICE);

        this->loadCallbacks();

        //Run Forrest, run!
        this->run();
    }
    catch(errorFlags e){
        switch(e){
            case PARSER_CANNOT_OPEN_FILE:
                logger::write("Cannot open file", E_ERROR);
                break;
            case WATCHDOG_CANNOT_OPEN_DIR:
                logger::write("Cannot open directory", E_ERROR);
                break;
            case WATCHDOG_CANNOT_FIND_CALLBACKS:
                logger::write("Cannot find callbacks", E_ERROR);
                break;
            case WATCHDOG_CANNOT_LOAD_CALLBACKS:
                logger::write("Cannot load callbacks", E_ERROR);
                break;
            case WATCHDOG_CANNOT_LOAD_LOGFILE:
                logger::write("Cannot load logfile", E_ERROR);
                break;
            case WATCHDOG_CANNOT_PROCESS_CALLBACK:
                logger::write("Cannot process callback", E_ERROR);
                break;
            //Callback abort
            case WATCHDOG_CALLBACK_ABORT:
                throw WATCHDOG_ABORT;
                break;
            case WATCHDOG_SIGINT:
                break;
            default:
                logger::write("Catched unhandled expcetion", E_ERROR);
                break;
        }

        logger::write("Aborting..", E_ERROR);
        logger::close();

        throw WATCHDOG_ABORT;
    }
}

watchdog::~watchdog()
{
    logger::close();
}

list<string> watchdog::findCallbacks(){
    logger::write("Finding configuration in " + this->callbackDir, E_NOTICE);

    DIR* dirHandler = NULL;
    struct dirent* directory;

    list<string> files;
    string file;

    if((dirHandler = opendir(this->callbackDir.c_str())) == NULL)
        throw WATCHDOG_CANNOT_OPEN_DIR;

    while((directory = readdir(dirHandler)) != NULL){
        file = string(directory->d_name);

        if(file.substr(file.find_last_of(".") + 1) == "ini")
            files.push_back(file);
    }

    closedir(dirHandler);

    return files;
}

void watchdog::loadCallbacks(){
    list<string> files = this->findCallbacks();

    if(files.size() == 0)
        throw WATCHDOG_CANNOT_FIND_CALLBACKS;

    logger::write("Finded "+ utility::ltos(files.size()) +" configs", E_NOTICE);

    iniFile file;
    string fileName;

    for(list<string>::iterator p = files.begin(); p != files.end(); p++){
        logger::write("Loading "+ (*p), E_NOTICE);

        fileName = this->callbackDir+(*p);

        //Loading callback file
        file = iniParser::load(fileName);

        //Valid callback file
        if(file.isSection("General") && file.getValue("General", "logfile") != "" && file.getValue("General", "name") != ""){
            //Create callback
            callback cb(*p, file.getValue("General", "name"), file.getValue("General", "logfile"));

            //Get optional data
            long position = (file.getValue("General", "position") == "")? 0 : utility::stol(file.getValue("General", "position").c_str());
            long size = (file.getValue("General", "size") == "")? 0 : utility::stol(file.getValue("General", "size").c_str());

            //Set optional data
            cb.setPosition(position);
            cb.setSize(size);

            list<string> sections = file.getSections();

            for(list<string>::iterator q = sections.begin(); q != sections.end(); q++){
                //General? who care
                if((*q) != "General" && (file.getValue(*q, "pattern") != "") && (file.getValue(*q, "callback") != ""))
                    cb.addTrigger(*q, file.getValue(*q, "pattern"), file.getValue(*q, "callback"));

            }

            //No valid trigger -> invalid callback file
            if(cb.getTriggersCount() == 0){
                logger::write("No valid triggers in "+ (*p) +", skipping", E_WARNING);
                continue;
            }

            this->suspects.insert(make_pair(file.getValue("General", "name"), cb));
        }
        //Invalid callback file!
        else{
            logger::write("Invalid callback file "+ (*p) +", skipping", E_WARNING);
            continue;
        }
    }

    if(this->suspects.size() == 0)
        throw WATCHDOG_CANNOT_LOAD_CALLBACKS;
}

void watchdog::saveCallbacks(){
    if(this->suspects.size() == 0)
        return;

    fstream* fileHandler;
    string file, name, logfile;
    long position, size;

    for(map<string, callback>::iterator p = this->suspects.begin(); p != this->suspects.end(); p++){
        logger::write("Saving "+ (*p).second.getFile(), E_NOTICE);

        file = this->callbackDir+((*p).second.getFile());
        name = (*p).second.getName();
        logfile = (*p).second.getLogfile();
        position = (*p).second.getPosition();
        size = (*p).second.getSize();

        fileHandler = new fstream(file.c_str(), ios::out);

        //Error - cant write to callback file
        if(!fileHandler->is_open()){
            delete fileHandler;
            continue;
        }

        //Write general informations to callback file
        (*fileHandler) << "[General]" << endl;
        (*fileHandler) << " name=" << name << endl;
        (*fileHandler) << " logfile=" << logfile << endl;
        (*fileHandler) << " position=" << position << endl;
        (*fileHandler) << " size=" << size << endl << endl;

        map<string, trigger> triggers = (*p).second.getTriggers();

        //Write all triggers to callback file
        for(map<string, trigger>::iterator q = triggers.begin(); q != triggers.end(); q++){
            (*fileHandler) << "[" << (*q).first << "]" <<endl;
            (*fileHandler) << " pattern=" << (*q).second.getPattern() << endl;
            (*fileHandler) << " callback=" << (*q).second.getCommand() << endl << endl;
        }

        fileHandler->close();
        delete fileHandler;
    }
}

void watchdog::run(){
    string logfile;
    long size, position, actualSize;

    fstream* fileHandler = NULL;

    //Inifinity
    while(true){
        if(!watchdog::active)
            throw WATCHDOG_SIGINT;

        //For all suspects
        for(map<string, callback>::iterator p = this->suspects.begin(); p != this->suspects.end(); p++){
            logfile = (*p).second.getLogfile();
            position = (*p).second.getPosition();
            size = (*p).second.getSize();

            logger::write("Checking " + logfile, E_NOTICE);

            //Opening logfile
            fileHandler = new fstream(logfile.c_str(), ios::in);

            if(!fileHandler->is_open()){
                delete fileHandler;
                throw WATCHDOG_CANNOT_LOAD_LOGFILE;
            }

            //Get actual size of logfile
            actualSize = utility::fileSize(logfile);

            //Logfile without any change
            if(size == actualSize){
                logger::write("No new records", E_NOTICE);

                fileHandler->close();
                delete fileHandler;
                continue;
            }
            //Logrotate
            else if(size > actualSize){
                logger::write("Logrotate, system pause, sleeping..", E_WARNING);

                fileHandler->close();
                delete fileHandler;

                sleep(10);

                (*p).second.setPosition(0);
                (*p).second.setSize(0);

                continue;
            }

            string line, command;
            unsigned int loadedLines = 0;
            unsigned int processedCallbacks = 0;
            regexApiMatch match;
            pid_t pid;

            //Set position to last position
            fileHandler->seekp(position, ios::beg);

            //Get all triggers
            map<string, trigger> triggers = (*p).second.getTriggers();

            //Loading lines from logfile
            while(getline(*fileHandler, line)){
                //Checking all triggers
                for(map<string, trigger>::iterator q = triggers.begin(); q != triggers.end(); q++){
                    //If regex match
                    if(regexApi::preg_match((*q).second.getPattern(), line, match)){
                        command = (*q).second.getCommand();

                        //Complete callback command
                        for(int i = 0; i < match.size(); i++)
                            command = utility::replace("\\" + utility::itos(i), match[i], command);

                        //Fork
                        if((pid = fork()) == -1){
                            logger::write("Cannot fork", E_ERROR);

                            fileHandler->close();
                            delete fileHandler;

                            throw WATCHDOG_CANNOT_PROCESS_CALLBACK;
                        }

                        //Calling callback
                        if(!pid){
                            execvp(command.c_str(), NULL);

                            fileHandler->close();
                            delete fileHandler;

                            throw WATCHDOG_CALLBACK_ABORT;
                        }

                        processedCallbacks++;
                    }
                }

                loadedLines++;
            }

            fileHandler->clear();

            logger::write("Loaded "+ utility::ltos(loadedLines) +" lines (callbacks: "+ utility::ltos(processedCallbacks) +")", E_NOTICE);

            actualSize = utility::fileSize(logfile);

            (*p).second.setPosition(fileHandler->tellg());
            (*p).second.setSize(actualSize);

            fileHandler->close();

            delete fileHandler;
        }

        this->saveCallbacks();

        //Soft kitty, warm kitty, little ball of fur..
        sleep(this->interval);
    }
}

void watchdog::getsig(int sig){
    watchdog::active = false;
}
