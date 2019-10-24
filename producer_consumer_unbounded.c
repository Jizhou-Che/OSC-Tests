// Single producer, single consumer, unbounded buffer.
// This program runs forever and has to be terminated manually.

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// The binary semaphore for synchronising access to the buffer.
sem_t sync;
// The binary semaphore for blocking the consumer.
sem_t delay_consumer;

// Number of items currently in the unbounded buffer.
int items = 0;

void * producer() {
	while (1) {
		sem_wait(&sync);
		// Produce an item.
		items++;
		printf("Produced, items = %d.\n", items);
		if (items == 1) {
			// Wake up the consumer if the buffer is no longer empty.
			sem_post(&delay_consumer);
		}
		sem_post(&sync);
	}
}

void * consumer() {
	// I will not start consuming before the producer starts producing.
	sem_wait(&delay_consumer);
	// Now I can start consuming.
	// Keep a local record of the number of items after the last time I consumed.
	int items_temp;
	while (1) {
		sem_wait(&sync);
		// Consume an item.
		items--;
		// Remember the number of items left after I have consumed that item.
		items_temp = items;
		printf("Consumed, items = %d.\n", items);
		sem_post(&sync);
		// Use this local copy to remind me whether I should go to sleep.
		// In case I get interrupted at this point and the number of items left is different when I come back.
		if (items_temp == 0) {
			// After consuming one there was none left.
			// I should sleep until producer wakes me up.
			sem_wait(&delay_consumer);
		}
	}
}

int main() {
	// Initialise the semaphores.
	sem_init(&sync, 0, 1);
	sem_init(&delay_consumer, 0, 0);
	
	// Create the producer and consumer threads.
	pthread_t tProducer, tConsumer;
	pthread_create(&tProducer, NULL, producer, NULL);
	pthread_create(&tConsumer, NULL, consumer, NULL);
	
	// Tell the main thread to wait for both threads to finish.
	pthread_join(tProducer, NULL);
	pthread_join(tConsumer, NULL);
	
	return 0;
}
