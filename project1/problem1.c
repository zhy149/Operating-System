#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#define BUFFER_SIZE 200

// Please compile this program with gcc and run the a.out file with the corresponding prompts
int main(){
	const char sp[2] = " "; // use space to split string, const var
	char *token; // token for parsing usage
	char cmd_str[BUFFER_SIZE];  // input command
	printf("Please input one command and options, use space to split command and options\n");
	gets(cmd_str); // copy stdin content into cmd_str
	int fork_id = fork(); // fork a new process, the cmd_str will be duplicated for both processes
	if (fork_id < 0){ // fork fail
		fprintf(stderr, "fork process failed\n");
		exit(1);
	}else if(fork_id==0){ // child proc
		int i = 0;
		char *my_args[20]; // 19 max options for each command
		token = strtok(cmd_str, sp);
		while(token != NULL){ // continue to parse
			my_args[i] = token;
			token = strtok(NULL, sp);
			i++; // move to next param
		}
		my_args[i] = NULL;
		execvp(my_args[0], my_args);
	}else{ // wait the child proc to finish
		wait(NULL); // wait the child to be finished
		printf("++++\n"); // finish running
	}
	return 0;
}	
