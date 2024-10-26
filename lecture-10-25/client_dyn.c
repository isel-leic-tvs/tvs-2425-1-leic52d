// client for libmath.so with dynamic linking in execution time
// observe smaps file in various pause points


#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

typedef int (*count_calls_func)();
typedef int (*add_func)(int, int);

void  suspend(char *msg) {
	printf("%s: press return to  continue...", msg);
	getchar();
}

int main() {
    printf("my pid is %d\n", getpid());
	suspend("start");
	
	void *lib = dlopen("./libmath.so", RTLD_LAZY);
	if (lib == NULL) {
		perror("error loading library");
		return 1;
	}
	
	suspend("after dl_open");
	
	add_func add  = (add_func) dlsym(lib, "add");
	if (add == NULL) { 
		perror("error getting func address");
		return 1;
	}
	
	suspend("symbol loaded");

	
	int v= add(2,3);
	printf("add(2,3)=%d\n", v);
	suspend("after call add");
	dlclose(lib);
	
	
	return 0;
}
