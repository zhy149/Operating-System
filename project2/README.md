Please read and follow the instructions to run the program correctly:

1. Please move the trace files into this folder, there is no trace files contained in this folder
2. The Makefile is contained and tested without any issue, please just type make in terminal under C4 Lab environment to compile the files.
3. After successful compilation, run the program with commonds like ./memsim tracefile.trace framenum algorithm mode
4. In the above command, tracefile.trace is any tracefile, framenum is an integer greater than 0, algorithm can be fifo, lru or vms, mode can be quiet or debug
5. If choose vms algorithm, then there will be another argument called percentage, which is an integer between 0 and 100, this argument is between algorithm and mode.
6. In quiet mode, only the result shows up, and the trace events show when choose debug mode.
7. Here are some running examples:
    ./memsim bzip.trace 64 vms 75 quiet
    output:
    sfifo policy
    fifo_mem_num 8
    lru_mem_num 24
    read_count_new 2168
    write_count 715
    total events 1000000

    ./memsim bzip.trace 64 fifo quiet
    output:
    fifo policy
    frame_num 64
    file_name bzip.trace
    read_count_new 1467
    write_count 514
    total events 1000000

    ./memsim bzip.trace 64 lru quiet
    output:
    lru policy
    read_count 1264
    write_count 420
    total events 1000000
