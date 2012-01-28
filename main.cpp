#include <iostream>
#include <cstring>
#include "watchdog.h"

string HELP_MESSAGE = "";

int main(int argc, char* argv[]){
    if(argc == 2 && strcmp("--help", argv[1]) == 0)
        cout << HELP_MESSAGE << endl;
    else{
        try{
            watchdog puppy("./callbacks/", 3);
        }
        catch(...){
            cerr << "Got exception" << endl;
        }
    }

    return 0;
}
