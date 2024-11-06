/*
 * implements an echo server using a ring buffer based memory channel
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "memchan.h"
#include "echo_service.h"
#include "../utils/chrono.h"
 
 
#define NITERS 1000

#define SHOW_RESP

int main(int argc, char *argv[]) {
	 
	
	mem_channel_t *channel = mc_open(ECHO_SERVICE_NAME);
	if (channel == NULL) {
		fprintf(stderr, "error opening server channel\n");
		return 1;
	}
	
	// create response channel
	 		
	char msg[MAX_MSG];
		
	chrono_t chron = chrono_start();
	for(int try = 1; try <= NITERS; try++) {
		sprintf(msg, "Hello_%d", try);
		mc_send_request(channel, msg, strlen(msg)+1);
		// get response
		mc_rcv_response(channel, msg);
#ifdef SHOW_RESP
		printf("%s\n", msg);
#endif
	}
	
	printf("%d tries in %ld micros!\n", NITERS, chrono_micros(chron));

	  	
	printf("client done!\n");
	return 0;
}
