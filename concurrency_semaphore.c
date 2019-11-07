// An approach to mutual exclusion using semaphore.
// The two threads now increment the counter to 100000000.
// Note that this implementation is even less efficient than the mutex implementation.
// This is because the enforcement of massive context switching generates even larger overhead than busy waiting.
// A much more efficient version of the increment function is implemented in the comments.
// This is achieved by performing less number of synchronisations and context switches.

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// Set each thread to increment the counter 50000000 times.
#define NUMBER_OF_INCREMENTS 50000000

// The semaphore.
sem_t s;

// The shared counter.
int counter = 0;

// Function to carry out inside each thread.
void * increment() {
	for (int i = 0; i < NUMBER_OF_INCREMENTS; i++) {
		// Atomic function to wait a semaphore.
		sem_wait(&s);
		// Critical section.
		counter++;
		// Atomic function to post a semaphore.
		sem_post(&s);
	}
	pthread_exit(NULL);
}
/*
void * increment() {
	// Do all the calculations locally and synchronise in the end.
	int temp = 0;
	for (int i = 0; i < NUMBER_OF_INCREMENTS; i++) {
		temp++;
	}
	// Atomic function to wait a semaphore.
	sem_wait(&s);
	// Critical section.
	counter += temp;
	// Atomic function to post a semaphore.
	sem_post(&s);
	pthread_exit(NULL);
}
*/

int main() {
	// Initialise internal value of semaphore to 1.
	sem_init(&s, 0, 1);
	
	// Create two threads.
	pthread_t tID1, tID2;
	pthread_create(&tID1, NULL, increment, NULL);
	pthread_create(&tID2, NULL, increment, NULL);
	
	// Tell the main thread to wait for all threads to finish.
	pthread_join(tID1, NULL);
	pthread_join(tID2, NULL);
	
	// Print the value of the counter.
	printf("The value of counter is: %d.\n", counter);
	
	return 0;
}
