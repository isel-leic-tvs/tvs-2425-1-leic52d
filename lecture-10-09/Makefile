all : scall_demo tst_scall scallint_demo

scall_demo: scall_demo.c mywrite.s 
	gcc -o scall_demo -g -Wall scall_demo.c mywrite.s -z noexecstack    
	
scallint_demo: scallint_demo.c mywrite32.s 
	gcc -o scallint_demo -g -Wall scallint_demo.c  mywrite32.s -z noexecstack -static  

tst_scall: tst_scall.c  mygetpid.s
	gcc -o tst_scall -g -Wall tst_scall.c  mygetpid.s chrono.c -z noexecstack 
	
clean:
	rm -rf scall_demo scallint_demo tst_scall *.o
