#pragma once

#define ECHO_DIR	"/tmp/"

#define ECHO_SERVICE_NAME ECHO_DIR "echo_unixsock_service"


#define MAX_MSG_SIZE 256


typedef struct  {
	char msg[MAX_MSG_SIZE];
} echo_msg_t;

 
