#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "chrono.h"

/**
 * this program tests the implementation
 * of our old int version of system call write (my_write)
 */

int my_write(int fd, const void *buf, int size);
 
char msg[] = "hello again!\n";

int main() {
	
	int res = my_write(1, msg, 13);
	printf("return of my_write is %d\n", res);

	return 0;
}
