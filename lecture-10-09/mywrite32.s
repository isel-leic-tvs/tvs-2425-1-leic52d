.text

.global my_write

/*
 * write system call using "int" instruction as
 * system call mechanism
 */

my_write:

    // adjust parameters for 32 bit call
    
	pushq %rbx
	movl %edi, %ebx
	movl %esi, %ecx
	
     
	movl $4, %eax
	int $128
	
	popq %rbx
	ret  

.end
