#include "dataStructures.h"
#include <iostream>
#include <list>
using namespace std;
 
int LRU_Memory::get_addr(){return addr;}
char LRU_Memory::get_mode(){return mode;}
int LRU_Memory::get_counter(){return counter;}
void LRU_Memory::set_addr(int new_addr){addr = new_addr;}
void LRU_Memory::set_mode(char new_mode){mode = new_mode;}
void LRU_Memory::increase_counter(){counter += 1;}
void LRU_Memory::clear_counter(){counter = 0;} // reset counter when a new page comes in
void LRU_Memory::set_counter(int new_counter){counter = new_counter;}