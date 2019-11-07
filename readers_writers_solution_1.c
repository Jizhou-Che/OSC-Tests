// The reader first solution to the readers and writers problem.
// As long as there are readers in existence, upcoming writers cannot start.
// Writers may starve.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Total number of readers and writers.
#define NUMBER_OF_THREADS 10000

// Mutex lock for mutual exclusion on reading or writing readerCount.
pthread_mutex_t lock;

// Binary semaphore for blocking readers or writers.
sem_t rwSync;

// The current number of readers.
int readerCount;

// The value to read and write.
int value;

void * reader() {
	pthread_mutex_lock(&lock);
	readerCount++;
	if (readerCount == 1) {
		// Block any upcoming writers if there are readers currently in existence.
		// Or go to sleep if there is a writer currently writing.
		sem_wait(&rwSync);
	}
	pthread_mutex_unlock(&lock);
	
	// The reading is outside the critical section so that multiple readers can be reading at the same time.
	printf("A reader reads %d.\n", value);
	
	pthread_mutex_lock(&lock);
	readerCount--;
	if (readerCount == 0) {
		// Unblock the writers if there is currently no reader.
		sem_post(&rwSync);
	}
	pthread_mutex_unlock(&lock);
	
	pthread_exit(NULL);
}

void * writer() {
	// Block any upcoming readers or writers if there is a writer currently writing.
	sem_wait(&rwSync);
	value = rand();
	printf("A writer writes %d.\n", value);
	// Unblock the readers or writers after I have done writing.
	sem_post(&rwSync);
	
	pthread_exit(NULL);
}

int main() {
	// Initialise the mutex and the semaphore.
	pthread_mutex_init(&lock, NULL);
	sem_init(&rwSync, 0, 1);
	
	// Create the reader and writer threads.
	pthread_t rwThreads[NUMBER_OF_THREADS];
	for (int i = 0; i < NUMBER_OF_THREADS; i++) {
		int threadType = rand() % 2;
		if (threadType == 0) {
			// Create a reader thread.
			pthread_create(&rwThreads[i], NULL, reader, NULL);
		} else {
			// Create a writer thread.
			pthread_create(&rwThreads[i], NULL, writer, NULL);
		}
	}
	
	// Tell the main thread to wait for all threads to finish.
	for (int i = 0; i < NUMBER_OF_THREADS; i++) {
		pthread_join(rwThreads[i], NULL);
	}
	
	printf("Final value: %d.\n", value);
	
	return 0;
}
