#include "readerwriter.h"

// Starter code from textbook that initialize the lock struct
void rwlock_init(rwlock_t *rw){
	rw->readers = 0;
	rw->writers = 0;
	sem_init(&rw->lock, 0, 1);
	sem_init(&rw->writelock, 0, 1);
	sem_init(&rw->rwctr, 0, 1); // Initiate extra semaphore
}
// Starter code from the textbook.
void rwlock_acquire_readlock(rwlock_t *rw){
	sem_wait(&rw->rwctr);
	sem_wait(&rw->lock);
	rw->readers++;
	if(rw->readers == 1){
		sem_wait(&rw->writelock); // first reader acquires writelock
	}
	sem_post(&rw->lock);
	sem_post(&rw->rwctr);
	// Entering Critical Section
}
void rwlock_release_readlock(rwlock_t *rw){
	rw->readers = rw->readers-1;
	if(rw->readers == 0){
		sem_post(&rw->writelock); // last reader releases writelock
	}
}
// Starter code from the textbook.
void rwlock_acquire_writelock(rwlock_t *rw){
	sem_wait(&rw->rwctr);
	sem_wait(&rw->writelock); // waits for writing lock
	rw->writers = rw->writers+1;
	// Entering Critical Section
}
// Starter code from the textbook.
void rwlock_release_writelock(rwlock_t *rw){
	sem_post(&rw->writelock); // release write lock
	sem_post(&rw->rwctr); // release common lock
	rw->writers = rw->writers-1;
}
// Restart lock for each scenario
void rwlock_del(rwlock_t *rw){
	sem_destroy(&rw->lock);
	sem_destroy(&rw->rwctr);
	sem_destroy(&rw->writelock);
}
// Simulates critical section part
void Read_Write(int rw){  // rw is either 0 or 1, 0 means writing, 1 means reading
	// Sample code from project description
	if(rw==1){
		printf("Thread Currently Writing\n");
	}else if(rw==0){
		printf("Thread Currently Reading\n");
	}
	int y = 3, T=100, i, j;
	T = rand()%2000; // random num calculation
	for(i = 0; i < T; i++){
		for(j = 0; j < T; j++){
			y = i*j;
		}
	}
}
// Reader function
void *reader(void *arg){
	rwlock_t lock = *(rwlock_t *)arg;
	rwlock_acquire_readlock((rwlock_t *)arg);
	Read_Write(0); // Reading: 0
	rwlock_release_readlock((rwlock_t *)arg);
	printf("Reading finished\n");
}
// Writer function
void *writer(void *arg){
	rwlock_t lock = *(rwlock_t *)arg;
	rwlock_acquire_writelock((rwlock_t *)arg);
	Read_Write(1); //Writing: 1
	rwlock_release_writelock((rwlock_t *)arg);
	printf("Writing finished\n");
}
