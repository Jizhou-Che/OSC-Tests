// An approach to mutual exclusion using mutex.
// The two threads now increment the counter to 100000000.
// Note that this implementation is inefficient because of the busy waiting.

#include <stdio.h>
#include <pthread.h>

// Set each thread to increment the counter 50000000 times.
#define NUMBER_OF_INCREMENTS 50000000

// The mutex lock.
pthread_mutex_t lock;

// The shared counter.
int counter = 0;

// Function to carry out inside each thread.
void * increment() {
	for (int i = 0; i < NUMBER_OF_INCREMENTS; i++) {
		// Atomic instruction to acquire a mutex lock.
		pthread_mutex_lock(&lock);
		// Critical section.
		counter++;
		// Atomic instruction to release a mutex lock.
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(NULL);
}

int main() {
	// Initialise mutex.
	pthread_mutex_init(&lock, NULL);
	
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
