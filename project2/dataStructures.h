#ifndef LRU_MEMORY_H
#define LRU_MEMORY_H

class LRU_Memory{ // set up the memory
    private:
        int addr;
        char mode; 
        int counter; // track lru number
    public:
        int get_addr();
        char get_mode();
        int get_counter();
        void set_addr(int new_addr);
        void set_mode(char new_mode);
        void increase_counter();
        void clear_counter(); // reset counter when a new page comes in
        void set_counter(int new_counter);
        
};

#endif