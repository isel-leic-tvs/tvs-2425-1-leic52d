/*
 * implements an echo server using unix domain stream sockets
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>
#include "echo_service.h"

#include <syslog.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

// size of pending connections queue
#define BACKLOG 50

// globals
int srv_sock;
volatile bool terminated = false;

void init_log() {
	openlog("echod", LOG_PID, LOG_DAEMON);
}

void sigterm_handler(int sig) {
	terminated = true;
	close(srv_sock);
}

void init() {
	// save daemon pid
	char pidfile[128];
	sprintf(pidfile, "echo %d > %sechod.pid",  getpid(), ECHO_DIR);
	system(pidfile); 
	init_log();
	
	//handling sigterm
	signal(SIGTERM, sigterm_handler);
}


int create_bind_socket(const char* sock_name) {
	int sock;
	struct sockaddr_un srv_addr;
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock == -1) return -1;
	
	// remove old entry if exists.
    // This is needed since linux assumes that a bind already exists
    // with the requested name, even if this is not true ;(
	if (unlink(sock_name) == -1 && errno != ENOENT) {
		close(sock);
		return -2;
	}
	  
	/*  bind socket */
	memset(&srv_addr, 0, sizeof(struct sockaddr_un));
	srv_addr.sun_family = AF_UNIX;
	strncpy(srv_addr.sun_path, sock_name, sizeof(srv_addr.sun_path) - 1);
	
	if (bind(sock, (struct sockaddr *) &srv_addr, 
	               sizeof(struct sockaddr_un)) == -1) {
		close(sock);
		return -3;
	}
	
	return sock;
}

void process_connection(int cfd) {
	echo_msg_t msg;
	int nread;
	
	while ((nread = read(cfd, &msg, sizeof(echo_msg_t))) > 0) {
		write(cfd, &msg, nread);
	}
	close(cfd);		 
}

void* dispatch_connection(void *arg) {
	int cfd = (int)(size_t) arg;
	process_connection(cfd);
	return NULL;
}


void run() {
	int cli_sock; // connection socket
	
	syslog(LOG_INFO, "echo server started!");
	while (!terminated) {  
		struct sockaddr_un cli_addr;
		socklen_t addrlen = sizeof(struct sockaddr_un);
		cli_sock = accept(srv_sock, (struct sockaddr *)  &cli_addr, &addrlen);
		if (terminated) break;
		if (cli_sock == -1) {
			syslog(LOG_INFO, "error creating socket");
			break;
		}
	 	pthread_t cthread;
		pthread_create(&cthread, NULL, dispatch_connection, (void *) (size_t) cli_sock);
	 
	}
	syslog(LOG_INFO, "echo server terminated!");
}

int main(int argc, char *argv[]) {
	
	if (fork() == 0) {
		init();
		
		// create server socket
		if ((srv_sock = create_bind_socket(ECHO_SOCKET_NAME)) < 0) {
			syslog(LOG_INFO, "error creating socket\n");
			return 1;
		}
	 
		// set listen queue size
		if (listen(srv_sock, BACKLOG) == -1) {
			syslog(LOG_INFO, "error setting  listen queue size");
			return 2;
		}
		
		// run the server
		run();
		
		// cleanup
		//unlink(ECHO_SOCKET_NAME);
		if (!terminated)
			close(srv_sock);
		closelog();
	}
}


 
