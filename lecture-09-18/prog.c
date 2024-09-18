#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

const char info[] = "general informations";


char info2[1024];

int table[1024] = {1, 2, 3, 4, 5};

int global = 3;

void suspend(const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    vprintf(format, argList);
    va_end(argList);
    printf("press enter to continue");
    getchar();
}

int main(int argc, char *argv[]) {
    int local = 3;
    printf("&info=%p\n", info);
    printf("&info2=%p\n", info2);
    printf("&table=%p\n", table);
    printf("&global=%p\n", &global);
    printf("&main=%p\n", main);
    printf("&local=%p\n", &local);
    
    suspend("value of global before suspend is %d\n", global);
    
    if (argc == 2) {
		global = atoi(argv[1]);
	}
    
    suspend("value of global after suspend is %d\n", global);
 
    return 0;
}
