#include <iostream>
#include <strings.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "dataStructures.h"
#include "policies.h"
using namespace std;

void segmented_fifo(string filename, int frame_num, string mode, int percentage) // mode is either quiet or debug
{
    FILE *pFile = fopen(filename.c_str(), "r");
    int fifo_mem_num = static_cast<int>(frame_num*((100-percentage)/100.0));
    int lru_mem_num = frame_num-fifo_mem_num;
    cout << "fifo_mem_num " << fifo_mem_num << endl;
    cout << "lru_mem_num " << lru_mem_num << endl;
    int counter1 = 0; // track first fifo memory
    int counter2 = 0; // track second lru memory
    unsigned addr;
    char rw;
    int write_count = 0, read_count = 0;
    int event_in_trace = 0;
    bool flag1 = false; // flag for fifo
    bool flag2 = false; // flag for lru
    int tmp_num; // to record tmp number needed
    int addr_store, count_store;
    char mode_store;
    
    if(fifo_mem_num == frame_num){ // use fifo
        fclose(pFile);
        fifo(filename, frame_num, mode);
        return;
    }else if(lru_mem_num == frame_num){ // use lru
        fclose(pFile);
        lru(filename, frame_num, mode);
        return;
    }

    vector<int> fifo_memory(fifo_mem_num, -1); // declare first level memory - fifo
    vector<int> fifo_rwmem(fifo_mem_num, 'n');

    LRU_Memory lru_mem[lru_mem_num]; // declare second level memory - lru
    for(int i=0; i<lru_mem_num; i++){ // initialize
        lru_mem[i].set_addr(-1);
        lru_mem[i].set_mode('a');
        lru_mem[i].clear_counter();
    }

    while(fscanf(pFile,"%x %c",&addr,&rw)==2) // read in entries
    {
        int real_addr = (addr/4096);
        event_in_trace += 1; // new event
        flag1 = false;
        flag2 = false;
        if(mode=="debug"){
            cout << "this is event: " << event_in_trace << endl;
            cout << "address: " << real_addr << endl;
            cout << "read write mode is: " << rw << endl;
        }
        if(counter1<fifo_mem_num){ // first level memory hasn't been filled up
            for(int i=0; i<counter1; i++){
                if(fifo_memory.at(i)==real_addr){
                    if(fifo_rwmem[i]=='R'){
                        fifo_rwmem[i] = rw; // update to W
                    }
                    flag1 = true;
                    break;
                }
            }
            if(!flag1){ // not found in current memory
                if(mode=="debug"){
                    cout << "read counter increases by one" << endl;
                }
                read_count += 1;
                fifo_memory[counter1] = real_addr;
                fifo_rwmem[counter1] = rw;
                counter1 += 1;
            }
        }else{ // first memory filled up
            for(int i=0; i<counter1; i++){
                if(fifo_memory[i]==real_addr){ // found in the first level memory
                    flag1 = true;
                    if(fifo_rwmem[i]=='R'){
                        fifo_rwmem[i] = rw; // update to W
                    }
                    break;
                }
            }
            if(!flag1){ // not found in the first level memory
                if(counter2<lru_mem_num){ // second level memory hasn't been filled up
                    for(int i=0; i<counter2; i++){ 
                        if(lru_mem[i].get_addr()==real_addr){ // find the target, push back to top of first fifo mem, need to sort
                            addr_store = lru_mem[i].get_addr();
                            mode_store = lru_mem[i].get_mode();
                            lru_mem[i].clear_counter();
                            lru_mem[i].set_addr(fifo_memory[0]);
                            lru_mem[i].set_mode(fifo_rwmem[0]);
                            for(int j=0; j<counter1-1; j++){
                                fifo_memory[j] = fifo_memory[j+1];
                                fifo_rwmem[j] = fifo_rwmem[j+1];
                            }
                            fifo_memory[counter1-1] = addr_store;
                            fifo_rwmem[counter1-1] = mode_store;
                            if(mode_store=='R'){ // optional
                                fifo_rwmem[counter1-1] = rw;
                            }
                            tmp_num = i; // mark this one
                            flag2 = true;
                            break;
                        }
                    }
                    if(!flag2){ // not found in secondary memory
                        read_count += 1; // add one more disk read
                        lru_mem[counter2].set_addr(fifo_memory[0]);
                        lru_mem[counter2].set_mode(fifo_rwmem[0]);
                        lru_mem[counter2].clear_counter(); // reset counter to 0
                        for(int j=0; j<counter1-1; j++){
                            fifo_memory[j] = fifo_memory[j+1];
                            fifo_rwmem[j] = fifo_rwmem[j+1];
                        }
                        fifo_memory[counter1-1] = real_addr;
                        fifo_rwmem[counter1-1] = rw;
                        counter2 += 1;
                        for(int i=0; i<counter2-1; i++){ // till counter
                            lru_mem[i].increase_counter();
                        }
                        if(mode=="debug"){
                            cout << "frame not filled, read_count plus one" << endl;
                        }
                    }else{ // found in secondary memory
                        if(mode=="debug"){
                            cout << "this is tmp_num: " << tmp_num << endl;
                        }
                        for(int i=0; i<counter2; i++){ // till counter+1
                            if(i != tmp_num){
                                lru_mem[i].increase_counter();
                            }
                        }
                        for(int j=tmp_num+1; j<counter2; j++){ // tmp_num+1
                            if(lru_mem[j-1].get_counter()<lru_mem[j].get_counter()){
                                addr_store = lru_mem[j-1].get_addr();
                                count_store = lru_mem[j-1].get_counter();
                                mode_store = lru_mem[j-1].get_mode();
                                lru_mem[j-1].set_addr(lru_mem[j].get_addr());
                                lru_mem[j-1].set_counter(lru_mem[j].get_counter());
                                lru_mem[j-1].set_mode(lru_mem[j].get_mode());
                                lru_mem[j].set_addr(addr_store);
                                lru_mem[j].set_counter(count_store);
                                lru_mem[j].set_mode(mode_store);
                            }
                        }
                    }
                }else{ // second level memory filled up
                    for(int i=0; i<counter2; i++){
                        if(lru_mem[i].get_addr()==real_addr){ // found in secondary memory, move to fifo memory
                            addr_store = lru_mem[i].get_addr();
                            mode_store = lru_mem[i].get_mode();
                            lru_mem[i].clear_counter();
                            lru_mem[i].set_addr(fifo_memory[0]);
                            lru_mem[i].set_mode(fifo_rwmem[0]);
                            for(int j=0; j<counter1-1; j++){
                                fifo_memory[j] = fifo_memory[j+1];
                                fifo_rwmem[j] = fifo_rwmem[j+1];
                            }
                            fifo_memory[counter1-1] = addr_store;
                            fifo_rwmem[counter1-1] = mode_store;
                            if(mode_store=='R'){ // optional
                                fifo_rwmem[counter1-1] = rw;
                            }
                            tmp_num = i; // mark this one
                            flag2 = true;
                            break;
                        }
                    }
                    if(!flag2){ // not found in second memory, evict the oldest lru page, move fifo page to lru
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
                        for(int i=0; i<counter2-1; i++){
                            lru_mem[i].set_addr(lru_mem[i+1].get_addr());
                            lru_mem[i].set_mode(lru_mem[i+1].get_mode());
                            lru_mem[i].set_counter(lru_mem[i+1].get_counter());
                            lru_mem[i].increase_counter(); // increase counter for all unread pages
                        }
                        lru_mem[counter2-1].set_addr(fifo_memory[0]); // evict the least recently used page
                        lru_mem[counter2-1].set_mode(fifo_rwmem[0]);
                        lru_mem[counter2-1].clear_counter();
                        for(int i=0; i<counter1-1; i++){
                            fifo_memory[i] = fifo_memory[i+1];
                            fifo_rwmem[i] = fifo_rwmem[i+1];
                        }
                        fifo_memory[counter1-1] = real_addr;
                        fifo_rwmem[counter1-1] = rw;
                    }else{ // found in second memory, increment counter value
                        for(int i=0; i<counter2; i++){
                            //cout << "number i increase counter " << i << endl;
                            if(i!=tmp_num){
                                lru_mem[i].increase_counter();
                            }
                        }
                        for(int j=tmp_num+1; j<counter2; j++){ // tmp_num+1
                            if(lru_mem[j-1].get_counter()<lru_mem[j].get_counter()){
                                addr_store = lru_mem[j-1].get_addr();
                                count_store = lru_mem[j-1].get_counter();
                                mode_store = lru_mem[j-1].get_mode();
                                lru_mem[j-1].set_addr(lru_mem[j].get_addr());
                                lru_mem[j-1].set_counter(lru_mem[j].get_counter());
                                lru_mem[j-1].set_mode(lru_mem[j].get_mode());
                                lru_mem[j].set_addr(addr_store);
                                lru_mem[j].set_counter(count_store);
                                lru_mem[j].set_mode(mode_store);
                            }
                        }
                    }
                }
            }else{
                for(int i=0; i<counter2; i++){ // increment lru memory counter
                    lru_mem[i].increase_counter();
                }
            }
            if(mode=="debug"){
                cout << "***********" << endl;
                for(int j=0; j<counter2; j++){
                    cout << "page " << j << ": ";
                    cout << "address: " << lru_mem[j].get_addr() << " ";
                    cout << "mode: " << lru_mem[j].get_mode() << " ";
                    cout << "counter: " << lru_mem[j].get_counter() << endl;
                }
                cout << "***********" << endl;
            }
        }
    }
    fclose(pFile);
    cout << "read_count_new " << read_count << endl;
    cout << "write_count " << write_count << endl;
    cout << "total events " << event_in_trace << endl;
}