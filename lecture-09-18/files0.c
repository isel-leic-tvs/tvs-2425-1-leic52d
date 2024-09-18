#include <stdio.h>
#include <unistd.h>

int main() {
   // mix fd and FILE output
   
   // use standard output via FILE*
   fputs("First", stdout);
   
   // what happens if this flush is commented?
   fflush(stdout);
   
   // use standard output via fd
   write(STDOUT_FILENO /* 1 */, "Second\n", 7);
   
   return 0;
}
