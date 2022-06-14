#ifndef READERWRITER_H
#define READERWRITER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>

/* Function initializationfrom textbook */
typedef struct _rwlock_t {
	sem_t lock; // reader lock
	sem_t writelock; // writerlock
	sem_t rwctr; // commonlock
	int writers; // reader num in cs
	int readers; // writer_num in cs
}rwlock_t;

void rwlock_init(rwlock_t *rw);
void rwlock_acquire_readlock(rwlock_t *rw);
void rwlock_release_readlock(rwlock_t *rw);
void rwlock_acquire_writelock(rwlock_t *rw);
void rwlock_release_writelock(rwlock_t *rw);
void rwlock_del(rwlock_t *rw);
/* Function calls from textbook */

void Read_Write(int rw);
void *reader(void *arg);
void *writer(void *arg);

#endif
