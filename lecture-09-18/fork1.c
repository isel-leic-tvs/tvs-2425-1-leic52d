#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

/**
 * In order to distinguish parent from chils executions,
 * fork return different values to both:
 * To the parent is returned the child process id (pid) 
 * and to the child is returned 0 (not a valid id)
 * 
 * The code below show this
 * 
 * Uncomment the  DELAYED_WAIT define and note (using command "ps -ax")
 * that lists all the existing processes,
 * that the child process remains (with state Defunct) while parent is suspended
 * 
 */

 


int main() {
	 
	pid_t child_pid;
	
	child_pid = fork();
	
	if (child_pid == -1) {
		perror("error on fork");
		exit(1);
	}
	else if (child_pid == 0) {
		// on child
		printf("I'm the child with pid %d\n", getpid());
		exit(0);
	}
	else {
		// on parent
		printf("I,m the parent and i created process %d\n", child_pid);
	}
	printf("press enter to continue");
	getchar();
	return 0; 
}
