// A solution to the dining philosophers problem.

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// The number of philosophers or forks.
#define N 5

// Define philosopher states as integers.
#define THINKING 1
#define HUNGRY 2
#define EATING 3

// The binary semaphore for synchronising access to the states of philosophers.
sem_t sync;
// Binary semaphores for blocking philosophers when they are unable to start eating.
// Initialised to 0.
sem_t delay_philosophers[N];

// Initialise philosopher states.
int states[N] = {THINKING, THINKING, THINKING, THINKING, THINKING};

// Try to get a philosopher to start eating.
// As this function involves checking and changing states of philosophers, it must be called inside the critical section.
void try_and_eat(int id) {
	// Get the IDs of my neighbours.
	int leftPhilosopher = (id + N - 1) % N;
	int rightPhilosopher = (id + 1) % N;
	
	// I can only start eating if none of my neighbours are currently eating.
	if (states[id] == HUNGRY && states[leftPhilosopher] != EATING && states[rightPhilosopher] != EATING) {
		// Start eating.
		states[id] = EATING;
		// If I am a philosopher who can start eating directly, I will not go to sleep.
		// If I am one of the hungry neighbours who were asleep but are now able to start eating, my thread should be waken up and continue.
		sem_post(&delay_philosophers[id]);
	}
}

void take_forks(int id) {
	sem_wait(&sync);
	// I am now hungry.
	states[id] = HUNGRY;
	// Try to start eating.
	try_and_eat(id);
	sem_post(&sync);
	// Go to sleep if I was not able to start eating.
	sem_wait(&delay_philosophers[id]);
}

void drop_forks(int id) {
	// Get the IDs of my neighbours.
	int leftPhilosopher = (id + N - 1) % N;
	int rightPhilosopher = (id + 1) % N;
	
	sem_wait(&sync);
	// Stop eating and start thinking.
	states[id] = THINKING;
	// Check whether my neighbours can now start eating if they are hungry.
	try_and_eat(leftPhilosopher);
	try_and_eat(rightPhilosopher);
	sem_post(&sync);
}

void * philosopher(void * id) {
	// Get the ID of the philosopher.
	int philosopherID = * ((int *) id);
	
	while (1) {
		printf("Philosopher %d is thinking.\n", philosopherID);
		printf("Philosopher %d is hungry.\n", philosopherID);
		take_forks(philosopherID);
		printf("Philosopher %d is eating.\n", philosopherID);
		drop_forks(philosopherID);
	}
}

int main() {
	// Initialisation of semaphores.
	sem_init(&sync, 0, 1);
	for (int i = 0; i < N; i++) {
		sem_init(&delay_philosophers[i], 0, 0);
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
