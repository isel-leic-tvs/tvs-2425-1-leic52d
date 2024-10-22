/**--------------------------------------------
 * 
 * using mmap to create an (anonymous)  memory region
 * shared (by inheritance) with the created child
 * 
 * observe the memory used by processas in respective smaps files
 * jmartins, october 2023
 *-------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>

#define REG_SIZE (1024*1024*2)

typedef uint8_t byte;


// para poder observar os efeitos no correspondente "smaps"
void  suspend(char *msg) {
	printf("%s: press return to  continue...", msg);
	getchar();
}

void fill_data(byte* data, int size, int val) {
	for(int i=0; i < size; ++i) {
		data[i] = val;
	}
}

int sum_data(byte* data, int size) {
	int sum = 0;
	for(int i=0; i < size; ++i) {
		sum += data[i];
	}
	return sum;
}

int main() {
 
	printf("my pid is %d\n", getpid());
	 
	suspend("start point!");
	
	// try change MAP_SHARED to MAP_PRIVATE and observe the changes in father and child smaps
	byte *mapbase = (byte*) mmap(NULL, REG_SIZE, PROT_WRITE | PROT_READ, 
                        MAP_ANONYMOUS | MAP_PRIVATE, -1 ,0);
	
	if (mapbase == MAP_FAILED) {
		perror("error mapping numbers file");
		return 3;
	}
	printf("memory papped at %p\n", mapbase);
	suspend("after memory mapping");
	
	// father filling the shared region with 1
	fill_data(mapbase, REG_SIZE, 1);
	
	suspend("after memory fill");
	
	int pid;
	if ((pid=fork()) == 0) {
		suspend("child: on start");
		
	 
		// child reading the shared region
		int res = sum_data(mapbase, REG_SIZE);
		printf("child: data region sum = %d\n", res);
	
		suspend("child: after data sum");
		
		// child partial write to shared region
		fill_data(mapbase, REG_SIZE/2, 2);
	 
		suspend("child: after data fill");
		exit(0);
	}
	else {
		printf("parent: child pid is %d\n", pid);
		wait(NULL);
	}
	suspend("parent: after child termination");
	
	// father reading the shared region after child termination
	int res = sum_data(mapbase, REG_SIZE);	
	printf("parent: data region sum = %d\n", res);
	
	suspend("parent: before termination");
	return 0;
	
}

