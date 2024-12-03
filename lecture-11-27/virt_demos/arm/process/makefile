all:	hello_arm64 hello_x86-64

hello_arm64: hello_arm64.s
	aarch64-linux-gnu-as hello_arm64.s -o asm64.o && aarch64-linux-gnu-ld asm64.o -o hello_arm64

hello_x86-64: hello_x86-64.s
	as hello_x86-64.s -o .s -o hello_x86-64.o && ld hello_x86-64.o -o hello_x86-64
	
	
clean:
	rm -rf hello_arm64 hello_x86-64 *.o

