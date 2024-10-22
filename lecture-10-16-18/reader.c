/**
 * the reader for example of named shared memory use via shm_open
 * 
 * observe smaps and shm object in /dev/shm
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "shmem_defs.h"	

void  suspend(char *msg) {
	printf("%s: press return to  continue...", msg);
	getchar();
}

int main(int argc, char *argv[]) {
	printf("my pid is %d\n", getpid());
 
	
    int fd = shm_open(SHMEM_NAME, O_RDONLY, 0);
    if (fd == -1) {
		perror("error opening shared mem object");
		return 1;
	}
	
	void *mapbase = mmap(NULL, SHMEM_SIZE, PROT_READ, MAP_SHARED, fd,0);
	if (mapbase == MAP_FAILED) {
		perror("error mapping numbers file");
		return 3;
	}
	
	printf("memory mapped at %p\n", mapbase);
	suspend("after file map");
	
	close(fd);
	
	int *ints = (int *) mapbase;
    
    printf("memory first value: %d\n", ints[0]);
	
	suspend("after value read");
	
	munmap(mapbase, SHMEM_SIZE);
	 
	suspend("after unmap shm region");
	return 0;
}
