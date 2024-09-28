#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define MAX_LINE 512

#define MATCH 0
#define UNMATCH 1
#define ERROR 2

typedef void (*FUNCTION)();
int  exec_test(FUNCTION func, char *expectedName);

void f1() {
    printf("I'm being tested!\n");
    printf("A second line\n");
    return;
}


/**
 * compares the content of the result output with the expected one
 */
bool check_same_content(char *expectedName, FILE *output) {
	char linee[MAX_LINE], lineo[MAX_LINE];
	FILE *expected = fopen(expectedName, "r");
	if (expected == NULL) {
		perror("error opening extected out file");
		return false;
	}
 	
	char *le, *lo;
	while(true) {
		le = fgets(lineo, MAX_LINE, output);
	    lo = fgets(linee, MAX_LINE, expected);
	    if (le == NULL || lo == NULL || strcmp(linee, lineo) != 0) {
			break;
		}
	}
	fclose(expected);
	return le == NULL && lo == NULL;
}

void show_file(FILE *f) {
    int  c;
    while((c= fgetc(f)) != EOF) {
        putchar(c);
    }
}

/**
 * The test function
 */
void test(FUNCTION func, char* expectedName) {
    int result = exec_test(func, expectedName);
    
    if (result == ERROR) {
        fprintf(stderr, "test function terminates abnormally\n");
    }
    else if (result == UNMATCH) {
		printf("different output from expected!\n");
	}
	else {
		printf("success!\n");
	}
  
}


/**
 * execute test and returns the result, wich could be:
 *    MATCH - the resulted output is the expected one
 *    UNMATCH - the test terminated normally, but the resulted output
 *              is different from the expected one
 *    ERROR   - the test terminated anormally
 */

int exec_test(FUNCTION func, char *expectedName) {
    
    int pid;
    int p[2];
    
    if (pipe(p) == -1) {
		perror("error creating pipe");
		return ERROR;
	}
	 
    FILE *output = fdopen(p[0], "r");
    
    if ((pid = fork()) == 0) { // child process
        close(p[0]);

        dup2(p[1], STDOUT_FILENO);
        close(p[1]);

        func();
        exit(0);
    }
    else if (pid > 0) { // parent process
		close(p[1]);
		
		bool equals = check_same_content(expectedName, output);
		fclose(output);
		
		int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
		  return equals ? MATCH : UNMATCH;
		}
	
      
    }
    return ERROR;
}


int main() {
    test(f1,"expected.txt" );
    return 0;
}
