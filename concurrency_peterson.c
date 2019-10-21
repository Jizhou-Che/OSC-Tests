// An approach to mutual exclusion using the Peterson's algorithm.
// Works only for two threads.
// Worked well on old machines.
// Not working perfectly on modern machines because the instructions may be carried out in a different order as they were written.
// The two threads should increment the counter to 100000000 but the actual result is a bit smaller.
// Note that this implementation is also inefficient because the busy waiting makes it not true parallelism.

#include <stdio.h>
#include <pthread.h>

// Set each thread to increment the counter 50000000 times.
#define number_of_increments 50000000

// The shared counter.
int counter = 0;

// Shared variables in Peterson's algorithm.
// Indicates which process is next to enter its critical section.
int turn;
// Indicates whether each process is ready to enter its critical section.
int flag[2];

// Function to carry out inside thread A.
void * increment_a() {
	// I am thread A.
	for (int i = 0; i < number_of_increments; i++) {
		// I want to enter my critical section.
		flag[0] = 1;
		// Let the other thread enter its critical section first.
		turn = 1;
		// Wait for the other thread to exit its critical section.
		while (flag[1] == 1 && turn == 1);
		// My critical section.
		counter++;
		// I no longer want to enter my critical section.
		flag[0] = 0;
	}
	pthread_exit(NULL);
}

// Function to carry out inside thread B.
void * increment_b() {
	// I am thread B.
	for (int i = 0; i < number_of_increments; i++) {
		// I want to enter my critical section.
		flag[1] = 1;
		// Let the other thread enter its critical section first.
		turn = 0;
		// Wait for the other thread to exit its critical section.
		while (flag[0] == 1 && turn == 0);
		// My critical section.
		counter++;
		// I no longer want to enter my critical section.
		flag[1] = 0;
	}
	pthread_exit(NULL);
}

int main() {
	// Create two threads.
	pthread_t tID1, tID2;
	pthread_create(&tID1, NULL, increment_a, NULL);
	pthread_create(&tID2, NULL, increment_b, NULL);
	
	// Tell the main thread to wait for all threads to finish.
	pthread_join(tID1, NULL);
	pthread_join(tID2, NULL);
	
	// Counter should be 100000000 but is actually a bit smaller.
	printf("The value of counter is: %d.\n", counter);
	
	return 0;
}
