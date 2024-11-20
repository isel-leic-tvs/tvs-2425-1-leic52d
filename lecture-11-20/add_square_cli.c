/*
 * implements an echo client using a unix domain stream socket
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>

 
#include "services.h"
 

int create_client_socket() {
	return socket(AF_UNIX, SOCK_STREAM, 0);
}


int main(int argc, char *argv[]) {
    int val1, val2;
	char c1, c2; 
	if (argc != 3 || 
        sscanf(argv[1], "%d%c", &val1, &c1) != 1 ||
        sscanf(argv[2], "%d%c", &val2, &c2) != 1) {
        fprintf(stderr, "usage: calc_cli <number>\n");
        return 1;
    }
    
	int cli_sock = create_client_socket();
 	
	if (cli_sock < 0) {
		fprintf(stderr, "calc_cli: error creating socket\n");
		return 1;
	}
	
	/* Construct server address, and make the connection */
	struct sockaddr_un srv_address;
	
	memset(&srv_address, 0, sizeof(struct sockaddr_un));
	srv_address.sun_family = AF_UNIX;
	strncpy(srv_address.sun_path, CALC_SOCK_NAME, 
			sizeof(srv_address.sun_path) - 1);
	
	if (connect(cli_sock, (struct sockaddr *) &srv_address, 
	          sizeof(struct sockaddr_un)) == -1) {
		fprintf(stderr, "calc_cli: error connecting socket\n");
		return 2;
	}
	
    // send request
    char msg[64];
    int num;
    int msg_size = sprintf(msg, "%d %d\n", val1, val2);
    write(cli_sock, msg, msg_size);

    read(cli_sock, msg, 32);
    
    if (sscanf(msg, "%d", &num) != 1) {
        printf("calc_cli: bad response number!\n");
    }
	else {
        printf("square(%d)+%d=%d\n", val1, val2, num);
    }
 
	close(cli_sock);
 
	return 0;
}
