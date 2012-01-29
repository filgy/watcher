#include <iostream>
#include <cstring>
#include "watchdog.h"

string HELP_MESSAGE = "Usage: watchdog [callbacksdir] [timeout]";

int main(int argc, char* argv[]){
    if(argc == 2 && strcmp("--help", argv[1]) == 0)
        cout << HELP_MESSAGE << endl;
    else{
        string callbackDir = (argc == 2)? argv[1] : "./callbacks/";
        unsigned int interval = (argc == 3)? atoi(argv[2]) : 5;

        if(interval == 0) interval++;

        try{
            watchdog puppy(callbackDir, interval);
        }
        catch(...){
            return 1;
        }
    }

    return 0;
}
