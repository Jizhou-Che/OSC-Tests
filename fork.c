#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	// The status of the parent process.
	int pStatus;
	
	// Create the child process.
	int pID = fork();
	
	// The parent has a process ID greater than 0.
	// The child has a process ID equal to 0.
	if (pID < 0) {
		printf("Fork error.\n");
	} else if (pID == 0) {
		// I am the child process.
		printf("Hello from the child process! Process ID: %d.\n", pID);
	} else {
		// I am the parent process.
		// Change the status of parent process to blocked until child process finishes.
		waitpid(pID, &pStatus, 0);
		printf("Hello from the parent process! Process ID: %d.\n", pID);
	}
	
	return 0;
}
