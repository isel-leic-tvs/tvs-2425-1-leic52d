
all: files0 files1 dup0 fork0 fork1 envs prog


files0: files0.c
	gcc -o files0 -Wall files0.c

files1: files1.c
	gcc -o files1 -Wall files1.c	
	
prog: prog.c
	gcc -o prog -Wall prog.c
	
dup0: dup0.c
	gcc -o dup0 -Wall dup0.c

fork0: fork0.c
	gcc -o fork0 -Wall fork0.c

fork1: fork1.c
	gcc -o fork1 -Wall fork1.c

	
envs: envs.c
	gcc -o envs -Wall envs.c
	
clean:
	rm -rf files0 files1 dup0 fork0 fork1  envs prog

