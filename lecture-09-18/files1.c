#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

/**
 * Este programa abre duas vezes o mesmo ficheiro
 * realizando escritas a partir dos dois descritores obtidos.
 * Observe o resultado.
 */

 

#define FILENAME "xx.txt"

//#define WITH_DUP  1

int main() {
	
	int fd;
	
	fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd == -1) {
		perror("creation error: ");
		exit(1);
	}
	write(fd, "1234\n", 5);
	int fd2;
#ifdef WITH_DUP
	fd2 =  dup(fd);
#else
	fd2 = open(FILENAME, O_RDWR);
	if (fd2 == -1) {
		perror("opening error: ");
		close(fd);
		exit(1);
	} 
#endif	
   /*
	char buffer[32];
	ssize_t nread = read(fd2, buffer, 16);
	if (nread == -1) {
		perror("error read file");
	}
	else {
		buffer[nread] = 0; // put string terminator
		printf("read %ld bytes: '%s'\n", nread, buffer);
	}
	

	nread = read(fd2, buffer, 16);
	buffer[nread] = 0; // put string terminator
	printf("read %ld bytes: '%s'\n", nread, buffer);
	*/
	
	write(fd2, "678\n", 4);
	
	
	close(fd);
	close(fd2);
	
	return 0;
	
}
