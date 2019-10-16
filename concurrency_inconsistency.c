#include <stdio.h>
#include <pthread.h>

// The shared counter.
int counter = 0;

// Function to carry out inside each thread.
void * increment(void * number_of_increments) {
	for (int i = 0; i < * ((int *) number_of_increments); i++) {
		counter++;
	}
	pthread_exit(NULL);
}

int main() {
	// Set each thread to increment the counter 50000000 times.
	int number_of_increments = 50000000;
	
	// Create two threads.
	pthread_t tID1, tID2;
	pthread_create(&tID1, NULL, increment, (void *) &number_of_increments);
	pthread_create(&tID2, NULL, increment, (void *) &number_of_increments);
	
	// Tell the main thread to wait for all threads to finish.
	pthread_join(tID1, NULL);
	pthread_join(tID2, NULL);
	
	// Counter should be 100000000 but is not.
	printf("The value of counter is: %d.\n", counter);
	
	return 0;
}
