#pragma once

#include <sys/types.h>

#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>

#define CHANNEL_NAME_MAX_SIZE 	 32
#define MAX_MSG			  	    512

typedef unsigned char byte;

typedef struct {
	pid_t ownerid;			    // the creator process
	sem_t request_available;    // signal a new request for server
	sem_t response_available;	// signal response avaiable for read
 
	// shared region size
	int msg_size;
	byte msg[MAX_MSG];	
} mem_channel_t;



/**
 * Create a single channel on shared memory
 */
mem_channel_t *mc_create(const char *name);
	 

/**
 * Opens a single channel on shared memory
 */
mem_channel_t *mc_open(const char *name);
	 
// client operations
bool mc_send_request(mem_channel_t *rb, void *item, int size);
int mc_rcv_response(mem_channel_t *rb, void *item_ptr);	

// server operations
int mc_rcv_request(mem_channel_t *rb, void *item_ptr);
bool mc_send_response(mem_channel_t *rb, void *item, int size);


void mc_destroy(mem_channel_t *rb, const char *name);
 
