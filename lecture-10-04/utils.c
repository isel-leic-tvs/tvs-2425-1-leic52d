#include <unistd.h>
#include "stdio.h" 
#include <stdarg.h>


void perror2(const char *format, ...) {
	char msg[256];
    va_list argList;
	
    va_start(argList, format);
    vsnprintf(msg, 256, format, argList);
    va_end(argList);
    perror(msg);
}
