.text

.global my_getpid


my_getpid:
 
	movl $20, %eax
	int $128

	ret  

.end
