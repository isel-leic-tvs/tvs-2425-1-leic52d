/**
 * implements an auto-activation "square server" 
 * using systemd like protocol.
 * Receives an integer operand on a line and 
 * give as response the square result
 */

#include <systemd/sd-daemon.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>
#include "services.h"
#include <fcntl.h>
#include <syslog.h>
#include <signal.h>
#include <stdbool.h>
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


int get_square(int num) {
	int cli_sock = socket(AF_UNIX, SOCK_STREAM, 0);
 	
	if (cli_sock < 0) {
		fprintf(stderr, "add_squared: error creating socket\n");
		return -1;
	}
	
	/* Construct server address, and make the connection */
	struct sockaddr_un srv_address;
	
	memset(&srv_address, 0, sizeof(struct sockaddr_un));
	srv_address.sun_family = AF_UNIX;
	strncpy(srv_address.sun_path, SQUARE_SOCK_NAME, 
			sizeof(srv_address.sun_path) - 1);
	
	if (connect(cli_sock, (struct sockaddr *) &srv_address, 
	          sizeof(struct sockaddr_un)) == -1) {
		fprintf(stderr, "add_squared: error connecting socket\n");
		return -2;
	}
	
	char msg[32];
	int msg_size = sprintf(msg, "%d\n", num);
  
    write(cli_sock, msg, msg_size);
    
	int square;
    read(cli_sock, msg, 32);
    close(cli_sock);
    if (sscanf(msg, "%d", &square) != 1) {
        return -3;
    }
	else {
#ifdef DEBUG
		fprintf(stderr, "add_squared: square(%d)=%d\n", num, square);
#endif
        return square;
    }
}

void process_connection(int cfd) {
	char msg[128];
	
	FILE *sock_stream = fdopen(cfd, "r+");
	while (fgets(msg, 128, sock_stream) != NULL) {
		int op1, op2;
		sscanf(msg, "%d%d", &op1, &op2);
		int square = get_square(op1);
		sprintf(msg, "%d\n", square + op2);
		fputs(msg, sock_stream);
		fflush(sock_stream);
	}
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
	fprintf(stderr, "add_squared: started!\n");
#endif
	while (!interrupted) {  
		struct sockaddr_un cli_addr;
		socklen_t addrlen = sizeof(struct sockaddr_un);
		cli_sock = accept(srv_sock, (struct sockaddr *)  &cli_addr, &addrlen);
		if (interrupted) break;
		if (cli_sock == -1) {
			perror("add_squared: error creating session socket");
			break;
		}
	 	pthread_t cthread;
		pthread_create(&cthread, NULL, dispatch_connection, (void *) (size_t) cli_sock);
	 
	}
#ifdef DEBUG
	fprintf(stderr, "add_squared: terminated!\n");
#endif
}

int main(int argc, char *argv[]) {
	
	init();
	
	
	if ((fcntl(3, F_GETFD))) {
		fprintf(stderr, "add_squared: baf file descriptor\n");
		exit(1);
	}
 
	// server socket from systemd launcher
	srv_sock = 3;
 

	// run the server
	run(srv_sock);
	
	// cleanup

	close(srv_sock);
	closelog();
	
}
