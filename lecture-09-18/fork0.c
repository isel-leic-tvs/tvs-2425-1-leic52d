/**
 * First example for creating a new process
 * 
 * "fork" is the Linux(Unix) API operation that
 * creates a new process, that is almost a pefect copy
 * of his parent. The memory state, the execution state, 
 * the active file descriptors, the environment variables, 
 * the current directory and other process state remains
 * equal in the child.
 * Of course, the process identifier and some statistic state,
 * as execution times, are different in the child.
 * 
 * As the execution state is the same in child, including the
 * Instruction Pointer (aka Program Counter), this means that 
 * both processes, parent and child, will execute the same code 
 * and have the same data after fork return.
 * 
 * This explains the output of next program.
 *
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
	int local = 5;
    printf("parent pid is %d\n", getpid());
    
    fork();
    fork();
    int x = 5;
    int* dyn = malloc(sizeof(int));
    *dyn = 10;
    printf("my pid is %d\n", getpid());
    
	printf("local is %d and located at %p on process %d\n", local, &local, getpid());
	printf("x is %d and located at %p on process %d\n", x, &x, getpid());
	printf("dyn is %d and located at %p on process %d\n", *dyn, dyn, getpid());

	return 0;
}
