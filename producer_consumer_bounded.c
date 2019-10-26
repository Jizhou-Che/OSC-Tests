// Multiple producer, multiple consumer, bounded buffer.
// This program runs forever and has to be terminated manually.

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// The size of the bounded buffer.
#define BUFFER_SIZE 100

// The binary semaphore for synchronising access to the buffer.
sem_t sync;
// The counting semaphore keeping track of the number of empty buffers.
sem_t empty;
// The counting semaphore keeping track of the number of full buffers.
sem_t full;

// Number of items currently in the bounded buffer.
int items = 0;

void * producer() {
	while (1) {
		// Go to sleep if no empty buffer, otherwise decrement the empty counter.
		sem_wait(&empty);
		sem_wait(&sync);
		// Produce an item.
		items++;
		printf("Produced, items = %d.\n", items);
		sem_post(&sync);
		// Increment the full counter, wake up a consumer if applicable.
		sem_post(&full);
	}
}

void * consumer() {
	while (1) {
		// Go to sleep if no full buffer, otherwise decrement the full counter.
		sem_wait(&full);
		sem_wait(&sync);
		// Consume an item.
		items--;
		printf("Consumed, items = %d.\n", items);
		sem_post(&sync);
		// Increment the empty counter, wake up a producer if applicable.
		sem_post(&empty);
	}
}

int main() {
	// Initialise the semaphores.
	sem_init(&sync, 0, 1);
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	
	// Create the producer and consumer threads.
	pthread_t tProducer1, tProducer2, tConsumer1, tConsumer2;
	pthread_create(&tProducer1, NULL, producer, NULL);
	pthread_create(&tProducer2, NULL, producer, NULL);
	pthread_create(&tConsumer1, NULL, consumer, NULL);
	pthread_create(&tConsumer2, NULL, consumer, NULL);
	
	// Tell the main thread to wait for all threads to finish.
	pthread_join(tProducer1, NULL);
	pthread_join(tProducer2, NULL);
	pthread_join(tConsumer1, NULL);
	pthread_join(tConsumer2, NULL);
	
	return 0;
}
