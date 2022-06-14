#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "dataStructures.h"
using namespace std;

void lru(string filename, int frame_num, string mode) // mode is either quiet or debug
{
    FILE *pFile = fopen(filename.c_str(), "r");
    unsigned addr;
    char rw;
    LRU_Memory lru_mem[frame_num]; // declare the memory
    int addr_store, count_store;
    char rw_store;
    int write_count = 0, read_count = 0;
    int event_in_trace = 0;
    int counter = 0; // track which one is empty yet
    bool flag1 = false;
    int tmp_num; // to record tmp number needed
    for(int i=0; i<frame_num; i++){ // initialize
        lru_mem[i].set_addr(-1);
        lru_mem[i].set_mode('a');
        lru_mem[i].clear_counter();
    }
    while(fscanf(pFile,"%x %c",&addr,&rw)==2)
    {
        int real_addr = (addr/4096);
        event_in_trace += 1; // new event
        flag1 = false;
        if(mode=="debug"){
            cout << "event " << event_in_trace << endl;
            cout << rw << " at address " << real_addr << endl;
        }
        if(counter<frame_num){ // there is still empty page in memory
            for(int i=0; i<counter; i++){
                if(lru_mem[i].get_addr()==real_addr){
                    lru_mem[i].clear_counter();
                    if(lru_mem[i].get_mode()=='R'){ // optional
                        lru_mem[i].set_mode(rw);
                    }
                    tmp_num = i; // mark this one
                    flag1 = true;
                    break;
                }
            }
            if(!flag1){ // not found in current memory
                read_count += 1; // add one more disk read
                lru_mem[counter].set_addr(real_addr);
                lru_mem[counter].set_mode(rw);
                lru_mem[counter].clear_counter(); // reset counter to 0
                counter += 1;
                for(int i=0; i<counter-1; i++){ // till counter
                    lru_mem[i].increase_counter();
                }
                if(mode=="debug"){
                    cout << "frame not filled, read_count plus one" << endl;
                }
            }else{
                if(mode=="debug"){
                    cout << "this is tmp_num: " << tmp_num << endl;
                }
                for(int i=0; (i<counter); i++){ // till counter+1
                    //cout << "number i increase counter " << i << endl;  update mode or not?
                    if(i != tmp_num){
                        lru_mem[i].increase_counter();
                    }
                }
            }
        }else{ // no empty page, need to evict the least recently used page
            for(int i=0; i<frame_num; i++){
                if(lru_mem[i].get_addr()==real_addr){
                    if(lru_mem[i].get_mode()=='R'){ // optional
                        lru_mem[i].set_mode(rw);
                    }
                    lru_mem[i].clear_counter(); // reset to zero
                    flag1 = true;
                    tmp_num = i;
                    break;
                }
            }
            if(!flag1){ // not found in current memory
                read_count += 1;
                if(mode=="debug"){
                    cout << "read_count plus one" << endl;
                }
                if(lru_mem[0].get_mode()=='W'){
                    write_count += 1;
                    if(mode=="debug"){
                        cout << "write_count plus one" << endl;
                    }
                }
                for(int i=0; i<frame_num-1; i++){
                    lru_mem[i].set_addr(lru_mem[i+1].get_addr());
                    lru_mem[i].set_mode(lru_mem[i+1].get_mode());
                    lru_mem[i].set_counter(lru_mem[i+1].get_counter());
                    lru_mem[i].increase_counter(); // increase counter for all unread pages
                }
                lru_mem[frame_num-1].set_addr(real_addr); // evict the least recently used page
                lru_mem[frame_num-1].set_mode(rw);
                lru_mem[frame_num-1].clear_counter();
            }else{
                for(int i=0; i<counter; i++){
                    //cout << "number i increase counter " << i << endl;
                    if(i!=tmp_num){
                        lru_mem[i].increase_counter();
                    }
                }
            }
        }
        if(flag1){
            for(int j=tmp_num+1; j<counter; j++){
                if(lru_mem[j-1].get_counter()<lru_mem[j].get_counter()){
                    addr_store = lru_mem[j-1].get_addr();
                    count_store = lru_mem[j-1].get_counter();
                    rw_store = lru_mem[j-1].get_mode();
                    lru_mem[j-1].set_addr(lru_mem[j].get_addr());
                    lru_mem[j-1].set_counter(lru_mem[j].get_counter());
                    lru_mem[j-1].set_mode(lru_mem[j].get_mode());
                    lru_mem[j].set_addr(addr_store);
                    lru_mem[j].set_counter(count_store);
                    lru_mem[j].set_mode(rw_store);
                }
            }
        }

        if(mode=="debug"){
            cout << "***********" << endl;
            for(int j=0; j<counter; j++){
                cout << "page " << j << ": ";
                cout << "address: " << lru_mem[j].get_addr() << " ";
                cout << "mode: " << lru_mem[j].get_mode() << " ";
                cout << "counter: " << lru_mem[j].get_counter() << endl;
            }
            cout << "***********" << endl;
        }
    }
    fclose(pFile);
    cout << "read_count " << read_count << endl;
    cout << "write_count " << write_count << endl;
    cout << "total events " << event_in_trace << endl;
}