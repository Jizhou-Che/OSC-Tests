// A naive solution to the dining philosophers problem.
// Will deadlock if every philosopher picks up the fork on the left and waits for the fork on the right to become available.

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// The number of philosophers or forks.
#define N 5

// Binary semaphores representing the availability of each fork.
sem_t forks[N];

void * philosopher(void * id) {
	// Get the ID of the philosopher.
	int philosopherID = * ((int *) id);
	// Get the IDs of the forks beside the philosopher.
	int leftFork = (philosopherID + N - 1) % N;
	int rightFork = philosopherID % N;
	
	while (1) {
		printf("Philosopher %d is thinking.\n", philosopherID);
		printf("Philosopher %d is hungry.\n", philosopherID);
		sem_wait(&forks[leftFork]);
		sem_wait(&forks[rightFork]);
		printf("Philosopher %d is eating.\n", philosopherID);
		sem_post(&forks[leftFork]);
		sem_post(&forks[rightFork]);
	}
}

int main() {
	// Initialise all forks to be available.
	for (int i = 0; i < N; i++) {
		sem_init(&forks[i], 0, 1);
	}
	
	// Use a fixed array of thread IDs instead of an incrementing temporary value as thread arguments.
	// Otherwise the threads will refer to a changing or invalid value in the main thread.
	int tIDs[N];
	for (int i = 0; i < N; i++) {
		tIDs[i] = i;
	}
	// Create all philosophers.
	pthread_t philosophers[N];
	for (int i = 0; i < N; i++) {
		pthread_create(&philosophers[i], NULL, philosopher, &tIDs[i]);
	}
	
	// Tell the main thread to wait for all threads to finish.
	for (int i = 0; i < N; i++) {
		pthread_join(philosophers[i], NULL);
	}
	
	return 0;
}
