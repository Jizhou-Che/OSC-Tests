#include <stdio.h>
#include <pthread.h>

#define NUM_OF_THREADS 10

// Function to carry out inside each thread.
void * hello(void * tid) {
	printf("Hello from thread %d!\n", * ((int *) tid));
	pthread_exit(NULL);
}

int main() {
	// Identifiers of created threads.
	pthread_t threads[NUM_OF_THREADS];
	
	// Using a fixed array of thread IDs instead of an incrementing value as thread arguments.
	// In case the thread scheduler runs multiple threads refering to the same value in the main thread.
	int tIDs[NUM_OF_THREADS] = {0};
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		tIDs[i] = i;
	}
	
	// Create threads.
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
