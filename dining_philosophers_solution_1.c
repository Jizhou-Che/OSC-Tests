// A solution to the dining philosophers problem.
// Limited parallelism as forks could be locked by semaphores unnecessarily.
// Two philosophers could be both hungry and sitting in the right positions but fails to eat at the same time.

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// The number of philosophers or forks.
#define N 5

// Counting semaphores representing the number of philosophers allowed to use forks at a time.
// Deadlock happens only when every philosopher has the left fork but not the right fork.
// If only N - 1 philosophers may use forks at the same time, the deadlock scenario could be completely avoided.
sem_t use_forks;
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
		// Check if I am allowed to use forks.
		sem_wait(&use_forks);
		// Check the avalibility of forks.
		sem_wait(&forks[leftFork]);
		sem_wait(&forks[rightFork]);
		printf("Philosopher %d is eating.\n", philosopherID);
		sem_post(&forks[leftFork]);
		sem_post(&forks[rightFork]);
		sem_post(&use_forks);
	}
}

int main() {
	// Initialise the maximum number of philosophers allowed to use forks at the same time to N - 1.
	sem_init(&use_forks, 0, N - 1);
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
