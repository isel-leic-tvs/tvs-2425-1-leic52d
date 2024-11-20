

CC = gcc
CFLAGS = -g -Wall
 
 
all: launch_toy square_cli add_square_cli squared add_squared

squared:	square_srv.c
	$(CC) -o squared $(CFLAGS) square_srv.c  
	
add_squared:	add_square_srv.c
	$(CC) -o add_squared  $(CFLAGS) add_square_srv.c  

launch_toy:	launch_toy.c
	$(CC) -o launch_toy $(CFLAGS) launch_toy.c  
	
square_cli:	square_cli.c
	$(CC) -o square_cli  $(CFLAGS) square_cli.c

add_square_cli:	add_square_cli.c
	$(CC) -o add_square_cli  $(CFLAGS) add_square_cli.c
 
install:
	./install.sh

uninstall:
	./uninstall.sh

clean:
	rm -rf *.o square_cli launch_toy add_squared add_square_cli squared
	
