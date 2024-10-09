#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "chrono.h"

/**
 * this program tests the implementation
 * of our version of system call write (my_write)
 */

int my_write(int fd, const void *buf, int size);
 


int main()
{
    char msg[] = "hello again!\n";
 
	int res = my_write(STDOUT_FILENO, msg, 13);
	printf("return of my_write is %d\n", res);

	 
	return 0;
}
