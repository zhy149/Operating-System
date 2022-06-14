#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "policies.h"
using namespace std;

int main (int argc, char* argv[])
{
    string tracefile, policy, mode;
    int nFrames, percentage;
    if (argc == 5){
        tracefile = argv[1];
        nFrames = atoi(argv[2]);
        policy = argv[3];
        mode = argv[4];
    }else if(argc == 6){
        tracefile = argv[1];
        nFrames = atoi(argv[2]);
        policy = argv[3];
        percentage = atoi(argv[4]);
        mode = argv[5];
    }else{
        cout<<"Not enought command line arguments. Please re-run the program.\n";
        return 0;
    } 

    if(policy=="lru"){
        cout << "lru policy" << endl;
        lru(tracefile, nFrames, mode);
    }else if(policy == "fifo"){
        cout << "fifo policy" << endl;
        fifo(tracefile, nFrames, mode);
    }else if(policy == "vms"){
        cout << "sfifo policy" << endl;
        segmented_fifo(tracefile, nFrames, mode, percentage);
    }
    return 0;
}