#include "watchdog.h"

watchdog::watchdog(string callbackDir, unsigned int interval){
    logger::getInstance()->open("./server.log");

    this->callbackDir = callbackDir;
    this->interval = interval;

    try{
        logger::getInstance()->write("Watchdog starting...", true);

        //Run Forrest, run!
        this->run();
        //this->DEBUG();

    }
    catch(errorFlags e){
        switch(e){
            case PARSER_CANNOT_OPEN_FILE:
            //case PARSER_CANNOT_MAKE_REGEX:
            case WATCHDOG_CANNOT_OPEN_DIR:
                break;
            case WATCHDOG_CANNOT_FIND_CALLBACKS:
            case WATCHDOG_CANNOT_LOAD_CALLBACKS:
                break;

            default:

                break;
        }
    }
}

watchdog::~watchdog()
{
    logger::getInstance()->close();
}

list<string> watchdog::findCallbacks(){
    logger::getInstance()->write("Finding configuration in " + this->callbackDir, true);

    DIR* dirHandler = NULL;
    struct dirent* directory;

    if((dirHandler = opendir(this->callbackDir.c_str())) == NULL)
        throw WATCHDOG_CANNOT_OPEN_DIR;

    list<string> files;
    string file;

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

    logger::getInstance()->write("Finded "+ utility::itos(files.size()) +" configs", true);

    iniFile file;
    string fileName;

    for(list<string>::iterator p = files.begin(); p != files.end(); p++){
        logger::getInstance()->write("Loading "+ (*p), true);

        fileName = this->callbackDir+(*p);

        //Loading callback file
        file = iniParser::load(fileName);

        //Valid callback file
        if(file.isSection("General") && file.getValue("General", "logfile") != "" && file.getValue("General", "name") != ""){
            //Create callback
            callback cb(*p, file.getValue("General", "name"), file.getValue("General", "logfile"));

            //Get optional data
            unsigned int position = (file.getValue("General", "position") == "")? 0 : utility::stoi(file.getValue("General", "position").c_str());
            unsigned int size = (file.getValue("General", "size") == "")? 0 : utility::stoi(file.getValue("General", "size").c_str());

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
                logger::getInstance()->error("No valid trigger in "+ (*p) + ", skipping", true);
                continue;
            }

            this->suspects.insert(make_pair(file.getValue("General", "name"), cb));
        }
        //Invalid callback file!
        else{
            logger::getInstance()->error("Invalid callback file "+ (*p) + ", skipping", true);
            continue;
        }
    }

    if(this->suspects.size() == 0)
        throw WATCHDOG_CANNOT_LOAD_CALLBACKS;
}

void watchdog::saveCallbacks(){
    if(this->suspects.size() == 0)
        return;

    string file, name, logfile;
    unsigned int position, size;

    fstream* fileHandler;

    for(map<string, callback>::iterator p = this->suspects.begin(); p != this->suspects.end(); p++){
        logger::getInstance()->write("Saving "+ (*p).second.getFile(), true);

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

        (*fileHandler) << "[General]" << endl;
        (*fileHandler) << " name=" << name << endl;
        (*fileHandler) << " logfile=" << logfile << endl;
        (*fileHandler) << " position=" << position << endl;
        (*fileHandler) << " size=" << size << endl << endl;

        map<string, trigger> triggers = (*p).second.getTriggers();
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
    this->loadCallbacks();

    if(this->suspects.size() == 0)
        return;

    while(true){
        string logfile;
        fstream* fileHandler = NULL;

        for(map<string, callback>::iterator p = this->suspects.begin(); p != this->suspects.end(); p++){
            logfile = (*p).second.getLogfile();

            logger::getInstance()->write("Checking " + logfile, true);


        }

        this->saveCallbacks();
        sleep(this->interval);
    }
}

void watchdog::DEBUG(){
    if(this->suspects.size() == 0)
        return;

    cout << "#------ DEBUG -------#" << endl;

    for(map<string, callback>::iterator p = this->suspects.begin(); p != this->suspects.end(); p++){
        cout << "Suspect: " << (*p).first << endl;
        callback tmp = (*p).second;

        cout << " - file: " << tmp.getFile() << endl;
        cout << " - name: " << tmp.getName() << endl;
        cout << " - logfile: " << tmp.getLogfile() << endl;
        cout << " - position: " << tmp.getPosition() << endl;
        cout << " - size: " << tmp.getSize() << endl;

        map<string, trigger> triggers = tmp.getTriggers();
        for(map<string, trigger>::iterator q = triggers.begin(); q != triggers.end(); q++){
            cout << "   [" << (*q).first << "]" <<endl;
            cout << "   - pattern: " << (*q).second.getPattern() << endl;
            cout << "   - command: " << (*q).second.getCommand() << endl;
        }

        cout << endl;
    }
}
