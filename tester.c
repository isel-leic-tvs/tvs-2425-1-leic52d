#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

typedef void (*FUNCTION)();
FILE * launch(FUNCTION func);

void f1() {
    printf("I,m being tested!\n");
    return;
}

void show_file(FILE *f) {
    int  c;
    while((c= fgetc(f)) != EOF) {
        putchar(c);
    }
}

void test(FUNCTION func) {
    FILE * output = launch(func);
    if (output == NULL) {
        fprintf(stderr, "test output unavaiable\n");
    }
    show_file(output);
    fclose(output);
}



FILE * launch(FUNCTION func) {
    
    int pid;
    FILE *file = tmpfile();
    if ((pid = fork()) == 0) { // child process
      
        int fd = fileno(file);

        dup2(fd, STDOUT_FILENO);

        func();

        fclose(file);
        exit(0);
    }
    else if (pid > 0) {
        waitpid(pid, NULL, 0);
        fseek(file, 0, SEEK_SET);
        return file;
    }
    return NULL;

}

int main() {
    test(f1);
    return 0;
}