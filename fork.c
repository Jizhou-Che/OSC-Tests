#include <stdio.h>

int main() {
	int iStatus;
	int iPID = fork();
	if(iPID < 0) {
		printf("Fork error.\n");
	} else if (iPID == 0) {
		// Child process.
		printf("Hello from the child!\n");
	} else {
		// Parent process.
		waitpid(iPID, &iStatus, 0);
		printf("Hello from the parent!\n");
	}
	return 0;
}
