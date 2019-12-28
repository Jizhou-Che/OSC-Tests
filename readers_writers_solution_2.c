// The writer first solution to the readers and writers problem.
// As long as there are writers in existence, upcoming readers cannot start.
// Readers may starve.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Total number of readers and writers.
#define NUMBER_OF_THREADS 10000

// Binary semaphore managing the registration of readers.
sem_t reader_register;
// Binary semaphore for blocking the resource from writing.
sem_t write_resource;

// Mutex locks for mutual exclusion on reading or writing registeredReaderCount and writerCount.
pthread_mutex_t lock_reader, lock_writer;

// The current number of registered readers.
int registeredReaderCount;
// The current number of writers in existence.
int writerCount;

// The value to read and write.
int value;

void * reader() {
	// Register a new reader if there is currently no writer in existence.
	sem_wait(&reader_register);
	pthread_mutex_lock(&lock_reader);
	registeredReaderCount++;
	if (registeredReaderCount == 1) {
		// Block the resource from writing if there are registered readers.
		sem_wait(&write_resource);
	}
	pthread_mutex_unlock(&lock_reader);
	// The registration finishes after the resource is guaranteed to be blocked from writing.
	sem_post(&reader_register);
	
	// The reading is outside the critical section so that multiple readers can read in parallel.
	printf("A reader reads %d.\n", value);
	
	pthread_mutex_lock(&lock_reader);
	registeredReaderCount--;
	if (registeredReaderCount == 0) {
		// Unblock the resource from writing after all registered readers have finished.
		sem_post(&write_resource);
	}
	pthread_mutex_unlock(&lock_reader);
	
	pthread_exit(NULL);
}

void * writer() {
	pthread_mutex_lock(&lock_writer);
	writerCount++;
	if (writerCount == 1) {
		// Block the registration of readers if there are writers currently in existence.
		sem_wait(&reader_register);
	}
	pthread_mutex_unlock(&lock_writer);
	
	// Block other writers while writing.
	sem_wait(&write_resource);
	value = rand();
	printf("A writer writes %d.\n", value);
	sem_post(&write_resource);
	
	pthread_mutex_lock(&lock_writer);
	writerCount--;
	if (writerCount == 0) {
		// Unblock the registration of readers after all writers have finished.
		sem_post(&reader_register);
	}
	pthread_mutex_unlock(&lock_writer);
	
	pthread_exit(NULL);
}

int main() {
	// Initialise the semaphores and mutexes.
	sem_init(&reader_register, 0, 1);
	sem_init(&write_resource, 0, 1);
	pthread_mutex_init(&lock_reader, NULL);
	pthread_mutex_init(&lock_writer, NULL);
	
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
