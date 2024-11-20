/**
 * implements an auto-activation "square server" 
 * using systemd like protocol.
 * Receives an integer operand on a line and 
 * give as response the square result
 */

 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>
#include "services.h"

#include <syslog.h>
#include <signal.h>
#include <stdbool.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>


//#define DEBUG

// size of pending connections queue
#define BACKLOG 128

// globals
volatile bool interrupted = false;
int srv_sock;
 
void sigterm_handler(int sig) {
	interrupted = true;
	close(srv_sock);
}

void init() {
	//handling sigterm
	signal(SIGTERM, sigterm_handler);
}



void process_connection(int cfd) {
	char line[128];
#ifdef DEBUG
    fprintf(stderr, "squared: start process connection\n");
#endif
	 
	FILE *sock_stream = fdopen(cfd, "r+");
	if (sock_stream == NULL) {
		  fprintf(stderr, "squared: error opening sock stream\n");
	}
	 
	while (fgets(line, 128, sock_stream) != NULL) {
#ifdef DEBUG
		fprintf(stderr, "squared: line '%s' read\n", line);
#endif
		fflush(stderr);
		int op;
		sscanf(line, "%d", &op);
		sprintf(line, "%d\n", op*op);
		fputs(line, sock_stream);
		fflush(sock_stream);
	}
#ifdef DEBUG
	fprintf(stderr, "squared: end  process connection\n");
#endif
	fclose(sock_stream);		 
}

void* dispatch_connection(void *arg) {
	int cfd = (int)(size_t) arg;
	process_connection(cfd);
	return NULL;
}


void run(int srv_sock) {
	int cli_sock; // connection socket

#ifdef DEBUG
	fprintf(stderr, "squared: started!\n");
#endif
	while (!interrupted) {  
		struct sockaddr_un cli_addr;
		socklen_t addrlen = sizeof(struct sockaddr_un);
		 
		cli_sock = accept(srv_sock, (struct sockaddr *)  &cli_addr, &addrlen);
		if (interrupted) break;
		if (cli_sock == -1) {
		
			perror("squared: error creating session socket");
				
			break;
		}
	 	pthread_t cthread;
		pthread_create(&cthread, NULL, dispatch_connection, (void *) (size_t) cli_sock);
	 
	}

}

int main(int argc, char *argv[]) {
	
	init();
	
	
	if ((fcntl(3, F_GETFD))) {
		fprintf(stderr, "squared: baf file descriptor\n");
		exit(1);
	}
 
	// server socket from systemd launcher
	srv_sock = 3;

	// run the server
	run(srv_sock);
	
	// cleanup

	close(srv_sock);
#ifdef DEBUG
	fprintf(stderr, "squared: terminated!\n");
#endif
}
