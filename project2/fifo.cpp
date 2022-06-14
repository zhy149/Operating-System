#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "dataStructures.h"
using namespace std;

void fifo(string filename, int frame_num, string mode)
{
    FILE *pFile = fopen(filename.c_str(), "r");
    vector<int> memory(frame_num, -1);
    vector<int> rwmem(frame_num, 'n');
    cout << "frame_num " << frame_num << endl;
    cout << "file_name " << filename << endl;
    unsigned addr;
    char rw;
    int write_count = 0, read_count = 0;
    int event_in_trace = 0;
    int counter = 0; // track which one is empty yet
    bool flag1 = false;
    int tmp_num; // to record tmp number needed

    while(fscanf(pFile,"%x %c",&addr,&rw)==2){
        int real_addr = (addr/4096);
        event_in_trace += 1; // new event
        flag1 = false;
        if(mode=="debug"){
            cout << "this is event: " << event_in_trace << endl;
            cout << "address: " << real_addr << endl;
            cout << "read write mode is: " << rw << endl;
        }
        if(counter<frame_num){ // there is still empty page in memory
            for(int i=0; i<counter; i++){
                if(memory.at(i)==real_addr){
                    if(rwmem[i]=='R'){
                        rwmem[i] = rw;
                    }
                    flag1 = true;
                    break;
                }
            }
            if(!flag1){ // not found in current memory
                if(mode=="debug"){
                    cout << "insert " << real_addr << "into " << counter << "th place" << endl;
                    cout << "read counter increases by one" << endl;
                }
                read_count += 1;
                memory[counter] = real_addr;
                rwmem[counter] = rw;
                counter += 1;
            }
        }else{ // no empty page, need to evict
            for(int i=0; i<frame_num; i++){
                if(memory.at(i)==real_addr){
                    flag1 = true;
                    tmp_num = i;
                    break;
                }
            }
            if(!flag1){ // not found in current memory
                if(mode=="debug"){
                    cout << "read counter increases by one" << endl;
                }
                read_count += 1;
                if(rwmem[0]=='W'){
                    if(mode=="debug"){
                        cout << "write counter increases by one" << endl;
                    }
                    write_count += 1;
                }
                for(int i=0; i<frame_num-1; i++){
                    memory[i] = memory[i+1];
                    rwmem[i] = rwmem[i+1];
                }
                memory[frame_num-1] = real_addr; // evict the first page
                rwmem[frame_num-1] = rw;
                
            }else{
                if(rwmem[tmp_num]=='R'){
                    rwmem[tmp_num] = rw;
                }
            }
        }
    }
    fclose(pFile);
    cout << "read_count_new " << read_count << endl;
    cout << "write_count " << write_count << endl;
    cout << "total events " << event_in_trace << endl;
}