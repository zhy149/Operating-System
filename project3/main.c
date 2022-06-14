#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>
#include "readerwriter.h"

rwlock_t lock; // initialize global lock

int main(){
	FILE* fptr;
	int read_write;
	int count = 1;
	char scenarios[15];
	//rwlock_t lock;	
	/* Checks for the Scenario text file */

	if((fptr=fopen("scenarios.txt", "r"))==NULL){
		fprintf(stderr, "The file cannot be found or opened\n");
		exit(1);
	}
		
	while(fgets(scenarios, 15, fptr)!=NULL){
		rwlock_init(&lock);
		int i = 0;
		int scene_len = strlen(scenarios); // strlen

		printf("\n\nScenario %d \n%s\n\n", count, scenarios);

		// At most 10 threads can be executed at the same time
		pthread_t threads[10];

		for(i = 0; i < scene_len-1; i++){
			if(scenarios[i] == 'w'){
				printf("Writer %d created\n", i);
				pthread_create(&threads[i], NULL, writer, (void *)&lock);
				//printf("Writer %d created\n", i);
			}else if(scenarios[i] == 'r'){
				printf("Reader %d created\n", i);
				pthread_create(&threads[i], NULL, reader, (void *)&lock);
				//printf("Reader %d created\n", i);
			}else{
				fprintf(stderr, "There is error in the scenario.txt, only r and w are allowed\n");
				exit(1);
			}
		}
		for(i = 0; i < scene_len-1; i++){
			pthread_join(threads[i], NULL);
			//printf("Reader/Writer %d Finished. \n", i);
		}
		count++;
		rwlock_del(&lock);
	}
	// close file
	fclose(fptr);

	// exit threads
	pthread_exit(NULL);

	return 0;
}
