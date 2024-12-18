/**-------------------------------------
 * Criação programática de uma região de memória 
 * para maper em meória o ficheiro "numbers.dat".
 * Observar as consequências no Resident Set do processo
 * nos vários pontos de paragem e a persistância dos dados 
 * no ficheiro dependeendo do mapeamento ser privado ou shared
 *
 * Jorge Martins, outubro de 2024
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "filespecs.h"

void  suspend(char *msg) {
	printf("%s: press return to  continue...", msg);
	getchar();
}

int main(int argc, char *argv[]) {
	printf("my pid is %d\n", getpid());
	int fd = open(INTS_FILENAME, O_RDWR);
	
	suspend("start!");
	if (fd < 0) {
		perror("error creating numbers file");
		return 1;
	}
	
	// find file size
	off_t size = lseek(fd, 0, SEEK_END);
	
	// forma alternativa (menos intrusiva) de obter a dimensão do ficheiro
	/*
		struct stat sbuf;
		if (fstat(fd, &sbuf) == -1) {
			perror("error getting file size");
			return 2;
		}
		printf("FILE SIZE =%ld\n", sbuf.st_size);
	*/
    
	printf("FILE SIZE =%ld\n", size);
	
	
	// DO MAP 
	// testar com MAP_SHARED e MAP_PRIVATE
	void *mapbase = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	
	if (mapbase == MAP_FAILED) {
		perror("error mapping numbers file");
		return 3;
	}
	
	printf("memory mapped at %p\n", mapbase);
	suspend("after file map");
	
	close(fd);
	
	int *ints = (int *) mapbase;
	
	for(int i=0; i < NUMBERS_COUNT; ++i) {
		printf("%d\n",ints[i]);
	}
	
	suspend("after file show");
	
	ints[NUMBERS_COUNT-1] = 1000;
	
	printf("ints[NUMBERS_COUNT-1] = %d\n", ints[NUMBERS_COUNT-1] );
	
	suspend("after file write");
	
	if (munmap(mapbase, size) == -1) {
		perror("error unmapping numbers file");
		return 4;
	}
	
	return 0;
}
