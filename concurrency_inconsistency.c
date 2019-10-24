// A demonstration of the inconsistency problem in concurrency.
// The two threads should increment the counter to 100000000 but the actual result is much smaller.

#include <stdio.h>
#include <pthread.h>

// Set each thread to increment the counter 50000000 times.
#define NUMBER_OF_INCREMENTS 50000000

// The shared counter.
int counter = 0;

// Function to carry out inside each thread.
void * increment() {
	for (int i = 0; i < NUMBER_OF_INCREMENTS; i++) {
		counter++;
	}
	pthread_exit(NULL);
}

int main() {
	// Create two threads.
	pthread_t tID1, tID2;
	pthread_create(&tID1, NULL, increment, NULL);
	pthread_create(&tID2, NULL, increment, NULL);
	
	// Tell the main thread to wait for all threads to finish.
	pthread_join(tID1, NULL);
	pthread_join(tID2, NULL);
	
	// Counter should be 100000000 but is actually much smaller.
	printf("The value of counter is: %d.\n", counter);
	
	return 0;
}
