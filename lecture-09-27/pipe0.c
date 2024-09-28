#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * this program uses a pipe to create a communication
 * channel between the child (the producer) and the parent (the consumer)
 */
 
#define MAX_MSG_SIZE 256

int main() {
	int p[2];

	if (pipe(p) == -1) {
		perror("error creating pipe!");
		exit(1);
	}
 
	int pchild = fork();
	if (pchild == -1) {
		perror("error creating child process");
		exit(2);
	}
	else if (pchild == 0) { // child process code
		char msg[128];
		close(p[0]); // o descritor de leitura do pipe não é necessário no filho
		sprintf(msg, "hello from child %d\n", getpid());
		write(p[1], msg, strlen(msg)+1);
		exit(0);
	}
	else { // parent code
		char msg[128];

		close(p[1]); // o pai não é escritor do pipe, tem de fechá-lo

		int nr;
		int total = 0;
		while((nr = read(p[0], msg + total, 8)) > 0) {
				total += nr;
		}

		close(p[0]); // leitura concluída
		
		waitpid(pchild, NULL, 0);
		 
		printf("father received: '%s'\n", msg);
	}
	return 0;
}
	
	
		
