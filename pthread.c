// A demonstration of creating threads.

#include <stdio.h>
#include <pthread.h>

#define NUM_OF_THREADS 10

// Function to carry out inside each thread.
void * hello(void * tID) {
	printf("Hello from thread %d!\n", * ((int *) tID));
	pthread_exit(NULL);
}

int main() {
	// Use a fixed array of thread IDs instead of an incrementing temporary value as thread arguments.
	// Otherwise the threads will refer to a changing or invalid value in the main thread.
	int tIDs[NUM_OF_THREADS] = {0};
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		tIDs[i] = i;
	}
	
	// Create threads.
	pthread_t threads[NUM_OF_THREADS];
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		if (pthread_create(&threads[i], NULL, hello, (void *) &tIDs[i]) != 0) {
			printf("Failed to create thread %d.\n", i);
			return 0;
		}
	}
	
	// Tell the main thread to wait for all threads to finish.
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	
	return 0;
}
