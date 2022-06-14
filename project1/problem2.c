#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#define WRITE_END 1
#define READ_END 0
#define BUFFER_SIZE 200 // use 200 to be buffer size of the string

// please compile this program with gcc and run the a.out file with corresponding prompts
int main(){
	const char sp[2] = " "; // use space to parse string
	int fd[2]; 
	int i = 0; // for later parsing usage
	char* my_args[20]; // maximum 19 options
	char cmd_str[BUFFER_SIZE];  // input string storage place
	char *token;	// token for parsing input
	printf("Please input commands and options, use | to split commands and space to split command with parameters\n");
	gets(cmd_str); // gets input from stdin
	pipe(fd); // build the pipe
	int fork_id = fork(); // fork new process
	if (fork_id < 0){ // fork fail
		fprintf(stderr, "fork failed\n");
		exit(1);
	}else if(fork_id==0){ // child process
		dup2(fd[WRITE_END], STDOUT_FILENO); // enable pipe write end
		close(fd[READ_END]); // close read end
		close(fd[WRITE_END]); // close write end
		token = strtok(cmd_str, sp); // start parsing
		while(token != NULL && strcmp(token, "|")!=0){ // store first command
			my_args[i] = token;
			token = strtok(NULL, sp);
			i++;
		}
		my_args[i] = NULL;
		execvp(my_args[0], my_args);	
	}else{ // parent process, wait the child to finished
		fork_id = fork();
		if(fork_id < 0){ // fail to fork
			fprintf(stderr, "fork failed\n");
			exit(1);
		}else if(fork_id == 0){
			dup2(fd[READ_END], STDIN_FILENO); // enable pipe read end using dup2
			close(fd[READ_END]); // close read end
			close(fd[WRITE_END]); // close write end
			token = strtok(cmd_str, sp); // start parsing
			while(token != NULL && strcmp(token, "|")!=0){
				token = strtok(NULL, sp);
			}
			token = strtok(NULL, sp);
			while(token != NULL){
				my_args[i] = token;
				token = strtok(NULL, sp);
				i++; // next parsing token
			}
			my_args[i] = NULL;
			execvp(my_args[0], my_args);
		}else{
			close(fd[READ_END]);
			close(fd[WRITE_END]);
			while(wait(NULL) > 0);
			printf("++++\n"); // finish running
		}
	}
	return 0;
}	
