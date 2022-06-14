#ifndef POLICIES_H
#define POLICIES_H

#include <iostream>
#include <strings.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "dataStructures.h"
using namespace std;

void fifo(string filename, int frame_num, string mode); // fifo policy, implemented in fifo.cpp
void lru(string filename, int frame_num, string mode); // lru policy, implemented in lru.cpp
void segmented_fifo(string filename, int frame_num, string mode, int percentage); // segmented fifo policy, implemented in vms.cpp

#endif

