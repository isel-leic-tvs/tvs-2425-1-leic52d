/*
 * implements an echo server using a ring buffer based memory channel
 */

#include <stdio.h>
#include <string.h>
#include "memchan.h"
#include <stdarg.h>
#include <stdlib.h>
#include "echo_service.h"

#define MAX_MSGS	100

void failure(const char *format, ...) {
	char msg[256];
	int n = sprintf(msg, "fatal error: ");
    va_list argList;
	
    va_start(argList, format);
    vsnprintf(msg+n, 256-n, format, argList);
    va_end(argList);
    fprintf(stderr, "%s\n", msg);
	exit(1);
}

int main(int argc, char *argv[]) {
    printf("memchannel size = %ld\n", sizeof(mem_channel_t));
	printf("server: start!\n");
	mem_channel_t *channel = mc_create(ECHO_SERVICE_NAME);
	if (channel == NULL) {
		failure("error creating memory channel");
	}
 	 
	while(true) {
		char req[MAX_MSG];
		int req_size = mc_rcv_request(channel, &req);
#ifdef SHOW_REQUEST
        printf("msg received: %s\n", req);
#endif
	    
		mc_send_response(channel, req, req_size);
	}
	
	mc_destroy(channel, ECHO_SERVICE_NAME);
	
	printf("server done!\n");
	return 0;
}
