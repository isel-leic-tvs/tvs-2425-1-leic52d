#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "chrono.h"

/**
 * this program tests the implementation
 * of our version of system call getpid using old "int" method (my_getpid)
 * versus the library implementation using "syscall"
 */

 
int my_getpid();

#define NTRIES 100000
 

int main()
{

	int pid1, pid2;

	chrono_t chrono = chrono_start();
	for (int i = 0; i < NTRIES; ++i)
	{
		pid1 = my_getpid();
	}

	printf("with my_getpid in %ld micros\n", chrono_micros(chrono));

	chrono = chrono_start();
	for (int i = 0; i < NTRIES; ++i)
	{
		pid2 = getpid();
	}
	printf("with getpid in %ld micros\n", chrono_micros(chrono));

	printf("pid with my_getpid is %d\n", pid1);
	printf("pid with getpid is %d\n", pid2);

	return 0;
}
