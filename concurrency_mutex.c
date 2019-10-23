// An approach to mutual exclusion using mutex.
// The two threads now increment the counter to 100000000.
// Note that this implementation is inefficient because of the busy waiting.

#include <stdio.h>
#include <pthread.h>

// Set each thread to increment the counter 50000000 times.
#define number_of_increments 20000000

// The mutex lock.
pthread_mutex_t lock;

// The shared counter.
int counter = 0;

// Function to carry out inside each thread.
void * increment() {
	for (int i = 0; i < number_of_increments; i++) {
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
	// Create five threads.
	pthread_t tID1, tID2, tID3, tID4, tID5;
	// Initialise mutex before creating threads.
	pthread_mutex_init(&lock, NULL);
	pthread_create(&tID1, NULL, increment, NULL);
	pthread_create(&tID2, NULL, increment, NULL);
	pthread_create(&tID3, NULL, increment, NULL);
	pthread_create(&tID4, NULL, increment, NULL);
	pthread_create(&tID5, NULL, increment, NULL);
	
	// Tell the main thread to wait for all threads to finish.
	pthread_join(tID1, NULL);
	pthread_join(tID2, NULL);
	pthread_join(tID3, NULL);
	pthread_join(tID4, NULL);
	pthread_join(tID5, NULL);
	
	// Counter should be 100000000 but is actually much smaller.
	printf("The value of counter is: %d.\n", counter);
	
	return 0;
}
