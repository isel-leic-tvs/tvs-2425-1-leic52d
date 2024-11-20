/**
 * create and listen mult an add UDS server sockets,
 * in order to auto activate them on client conections
 * using asynchronous I/O (epoll)
 * 
 * jmartins, nov 2024
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "services.h"

typedef struct {
    char *sock_name;
    int sock_fd;
    char *srv_path;
    pid_t srv_pid;
} srv_map_entry_t;

//#define DEBUG

// globals
int epoll_fd;
volatile bool interrupted;

#define NENTRIES 2
srv_map_entry_t srv_mapper[NENTRIES];


void sigterm_handler(int sig) {
	interrupted = true;
	close(epoll_fd);
}


/**
 * Auxiliary fynctions to add/remove antery to/from epoll interest set
 */

static int add_fd(int fd, srv_map_entry_t *entry) {
	
	struct epoll_event evd;  
 	evd.data.ptr = entry;
	evd.events = EPOLLIN | EPOLLERR;
	
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &evd) == -1) {	 
		return -1;
	}
	 
	return 0;
}

static  int rm_fd(int fd) {
	struct epoll_event evd;  
	 
	return epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &evd);
}

srv_map_entry_t* entry_by_pid(pid_t pid) {
	for(int i=0; i < NENTRIES; ++i) {
		if (srv_mapper[i].srv_pid == pid) {
			return srv_mapper + i;
		}
	}
	return NULL;
}

void sigchld_handler(int sig) {
	 pid_t pid;
#ifdef DEBUG
	 fprintf(stderr, "launcher: sig child!\n");
#endif
	 while ((pid = waitpid(0, NULL, WNOHANG)) > 0) {
		srv_map_entry_t* entry = entry_by_pid(pid);
		if (entry != NULL) {
			// add respective socket to epoll interest set again
#ifdef DEBUG
			fprintf(stderr, "%s died!\n", entry->srv_path);
#endif
			entry->srv_pid = -1;
			add_fd(entry->sock_fd, entry);			
		}
	 } 
}

/**
 * Create and bind an unix domain stream socket
 * given it's name
 */
int create_bind_socket(const char* sock_name) {
	int sock;
	struct sockaddr_un srv_addr;
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock == -1) return -1;
	
	// remove old entry if exists
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
	
	// set listen queue size
	if (listen(sock, 128) == -1) {
		 
		return -4;
	}
	return sock;
}

/**
 * 
 */
bool init_srv_entry(srv_map_entry_t *entry, char *sock_name, char *srv_path) {
	int sock_fd  = create_bind_socket(sock_name);
	if (sock_fd == -1) {
		goto init_entry_error;
	}
	
	// add to epoll interest set
	if (add_fd(sock_fd, entry) == -1) {
		goto init_entry_error;
	}
	 
	entry->srv_path = srv_path;
	entry->srv_pid = -1;
	entry->sock_name = sock_name;
	entry->sock_fd = sock_fd;
	return true;
init_entry_error:

	if (sock_fd != -1) {
		close(sock_fd);
	}
	return false;
}

int init()  {
	//handling sigterm
	signal(SIGTERM, sigterm_handler);
	signal(SIGCHLD, sigchld_handler);

	if ((epoll_fd = epoll_create1(0)) == -1) {
		perror("error creating epoll object");
		return -1;
	}
	if (!init_srv_entry(srv_mapper, SQUARE_SOCK_NAME, SQUARE_SRV_PATH)) {
		close(epoll_fd);
		return -2;
	} 
	if (!init_srv_entry(srv_mapper+1, CALC_SOCK_NAME, CALC_SRV_PATH)) {
		rm_fd(srv_mapper[0].sock_fd);
		close(epoll_fd);
		return -3;
	} 
	return 0;
}

/**
 * activates the server when a client connection happens
 * the server receive the accept socket at fd 3.
 */
void activate_srv(srv_map_entry_t *entry) {
	// remove the entry from epoll interest set
	int sock_fd = entry->sock_fd;
	rm_fd(sock_fd);
	pid_t pid;
	if ((pid = fork() )== 0) {
		signal(SIGCHLD, SIG_DFL);
		// duplicate socket on fd 3
		dup2(sock_fd, 3);
		close(sock_fd);
		execl(entry->srv_path, entry->srv_path, NULL);
        
	}
	else {
		entry->srv_pid = pid;
	}
	 
}


void terminate_active_servers() {
	for(int i=0; i < NENTRIES; ++i) {
		if (srv_mapper[i].srv_pid != -1) {
			kill(srv_mapper[i].srv_pid, SIGTERM);
		}
	}
}

/**
 * waits and process the ready epoll set
 * return AIO_SYNCH_OPER if one of them belongs to the current thread.
 * (wich means the operation can immediatelly complete without blocking)
 */ 
static void process() {
    struct epoll_event rdy_set[2]; 	// for ready set notifications

	
  
    while(true) {
		int nready = epoll_wait(epoll_fd, rdy_set, 2, -1);
	    if (nready == -1) {
			if (errno == EINTR) continue;
#ifdef DEBUG
			perror("launcher: epoll_wait error");
#endif
			break;
		}
#ifdef DEBUG
		fprintf(stderr, "launcher: %d accepts available!\n", nready);
#endif
		for(int i= 0;  i < nready ; ++i) {
			struct epoll_event *evd = rdy_set+i;  
			srv_map_entry_t *entry = (srv_map_entry_t*) evd->data.ptr;
			if (entry->srv_pid == -1) {
	 			activate_srv(entry);
			}
		}	
	
	}
	terminate_active_servers();
	 
}


int main() {
	int res;
	if ((res = init()) != 0) {
		fprintf(stderr, "error %d creating epoll or epoll interest set\n", res);
	}
	process();
#ifdef DEGUB
    fprintf(stderr, "launch_toy: terminated!\n");
#endif
}
