#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_OF_PROCESSES 5

int main() {
	// Process identifiers of the child processes.
	int pIDs[NUM_OF_PROCESSES];
	
	for (int i = 0; i < NUM_OF_PROCESSES; i++) {
		// Create a new child process.
		pIDs[i] = fork();
		
		if (pIDs[i] < 0) {
			printf("Fork error.\n");
		} else if (pIDs[i] == 0) {
			// I am the child process.
			printf("Hello from child process no.%d! Process ID: %d.\n", i, getpid());
			// Exit immediately after my work is done.
			return 0;
		} else {
			// I am the parent process.
			printf("Process ID of child process no.%d is %d.\n", i, pIDs[i]);
		}
	}
	
	// Wait for all child processes to finish.
	while (wait(NULL) > 0);
	// Print the process ID of the parent process.
	printf("Hello from parent process! Process ID: %d.\n", getpid());
	
	return 0;
}
