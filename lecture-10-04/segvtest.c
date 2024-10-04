/**
 * A segmentation violation scenario due 
 * to strtok use on a read/only string
 * 
 * We caught the SIGSEV signal but theres is nothing
 * we can do here to remedy the situation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <setjmp.h>

typedef void (*sighandler_t)(int);


void sig_handler(int sig) {
    printf("SIGSEV caught!\n");
    exit(0);
}


int main() {
    char *msg = "not a good day";
    
    sighandler_t old = signal(SIGSEGV, sig_handler);
    if (old == SIG_DFL) printf("old behaviour is SIG_DFL\n");
    else if (old == SIG_IGN) printf("old behaviour is SIG_IGN\n");
    else printf("old was another handler at %p\n", old);
    
    
    char* tok = strtok(msg, " ");
    printf("first strtok done!\n");
    while(tok != NULL) {
        printf("tok=%s\n", tok);
        tok = strtok(NULL, " ");
    }
    
    printf("done!\n");
    return 0;
}
