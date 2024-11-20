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
    int num; 
	if (argc != 2 || sscanf(argv[1], "%d", &num) != 1) {
        fprintf(stderr, "usage: square_cli <number>\n");
        return 1;
    }
    
	int cli_sock = create_client_socket();
 	
	if (cli_sock < 0) {
		fprintf(stderr, "square_cli: error creating socket\n");
		return 1;
	}
	
	/* Construct server address, and make the connection */
	struct sockaddr_un srv_address;
	
	memset(&srv_address, 0, sizeof(struct sockaddr_un));
	srv_address.sun_family = AF_UNIX;
	strncpy(srv_address.sun_path, SQUARE_SOCK_NAME, 
			sizeof(srv_address.sun_path) - 1);
	
	if (connect(cli_sock, (struct sockaddr *) &srv_address, 
	          sizeof(struct sockaddr_un)) == -1) {
		fprintf(stderr, "square_cli: error connecting socket\n");
		return 2;
	}
	
	char msg[64];
    int msg_size = sprintf(msg, "%d\n", num);
    int square;
    
    write(cli_sock, msg, msg_size);

    read(cli_sock, msg, 32);
    
    if (sscanf(msg, "%d", &square) != 1) {
        printf("square_cli: bad response number!\n");
    }
	else {
        printf("square(%d)=%d\n", num, square);
    }
 
	close(cli_sock);
 
	return 0;
}
