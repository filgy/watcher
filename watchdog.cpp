#include "watchdog.h"

watchdog::watchdog(string callbackDir){
    this->callbackDir = callbackDir;

    try{
        this->loadCallbacks();

        this->DEBUG();
    }
    catch(...){

    }
}

watchdog::~watchdog()
{
    //dtor
}

list<string> watchdog::findCallbacks(){
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

    iniFile file;
    string fileName;

    for(list<string>::iterator p = files.begin(); p != files.end(); p++){
        fileName = this->callbackDir+(*p);

        //Loading callback file
        file = iniParser::load(fileName);

        //Valid callback file
        if(file.isSection("General") && file.getValue("General", "logfile") != "" && file.getValue("General", "name") != ""){
            //Create callback
            callback cb(file.getValue("General", "name"), file.getValue("General", "logfile"));

            //Get optional data
            unsigned int position = (file.getValue("General", "position") == "")? 0 : (unsigned int)atoi(file.getValue("General", "position").c_str());
            unsigned int size = (file.getValue("General", "size") == "")? 0 : (unsigned int)atoi(file.getValue("General", "size").c_str());

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
                continue;
            }

            this->suspects.insert(make_pair(file.getValue("General", "name"), cb));
        }
        //Invalid callback file!
        else{
            continue;
        }
    }
}

void watchdog::DEBUG(){
    if(this->suspects.size() == 0)
        return;

    cout << "#------ DEBUG -------#" << endl;

    for(map<string, callback>::iterator p = this->suspects.begin(); p != this->suspects.end(); p++){
        cout << "Suspect: " << (*p).first << endl;
        callback tmp = (*p).second;

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
