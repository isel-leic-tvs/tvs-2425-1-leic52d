#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "memchan.h"
#include "memutils.h"

 
mem_channel_t *mc_create(const char *name) {
 
	mem_channel_t *mc;
	 	
	mc = (mem_channel_t *) shmem_create(name, sizeof(mem_channel_t)); 
	if (mc == NULL) return NULL;
	if (sem_init(&mc->request_available, true, 0) != 0) {
		shmem_destroy(name, mc, sizeof(mem_channel_t));
		return NULL;
	}
	if (sem_init(&mc->response_available, true, 0) != 0) {
		sem_destroy(&mc->request_available);
		shmem_destroy(name, mc, sizeof(mem_channel_t));
		return NULL;
	}
	mc->ownerid = getpid();
	return mc;
}

/**
 * Opens an existing channel on shared memory
 */
mem_channel_t *mc_open(const char *name) {	
	return shmem_open(name);
}


/**
 * 
 * Send a request to server
 */
bool mc_send_request(mem_channel_t *mc, void *item, int size) {
	 
	if (size > MAX_MSG) {
		return false;
	} 	

	// copy the value to channel
	memcpy(mc->msg, item, size);
	mc->msg_size = size;
	// notify receiver
	sem_post(&mc->request_available);
	
	return true;
}

int mc_rcv_request(mem_channel_t *mc, void *item_ptr) {
	// wait for items avaiable
	sem_wait(&mc->request_available);
	memcpy(item_ptr, mc->msg, mc->msg_size);
	 
	 
	return mc->msg_size;
}

bool mc_send_response(mem_channel_t *mc, void *item, int size) {
	 
	// copy the value to buffer
	if (size > MAX_MSG) {
		return false;
	} 	
	memcpy(mc->msg, item, size);
	mc->msg_size = size;
	// notify client
	sem_post(&mc->response_available);
	
	return true;
}

int mc_rcv_response(mem_channel_t *mc, void *item_ptr) {
	// wait for response
	sem_wait(&mc->response_available);
	memcpy(item_ptr, mc->msg, mc->msg_size);
	
	return mc->msg_size;
}

void mc_destroy(mem_channel_t *mc, const char *name) {
	if (mc == NULL) return;
	if (mc->ownerid != getpid()) {
		munmap(mc, sizeof(mem_channel_t));
		return;
	}
	sem_destroy(&mc->request_available);
	sem_destroy(&mc->response_available);
	 
	shmem_destroy(name, mc, sizeof(mem_channel_t));
}
