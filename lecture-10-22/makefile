all: ibmath.so  client client_dyn   

CFLAGS= -g -Wall 
	
libmath.so: math.c
	gcc -o libmath.so $(CFLAGS) -fpic math.c -shared
		
client: client.c
	#gcc -o client $(CFLAGS) client.c ./libmath.so
	#gcc -o client $(CFLAGS) client.c '-Wl,-rpath,$$ORIGIN/.' libmath.so
	gcc -o client $(CFLAGS) client.c  -lmath


client_dyn: client_dyn.c
	gcc -o client_dyn $(CFLAGS) client_dyn.c

	
clean:
	rm -rf libmath.so errno client client_dyn 

prepare:
	sudo sh -c "echo /usr/local/lib  > /etc/ld.so.conf.d/tvs.conf"
	sudo ldconfig
	
install:
	sudo cp libmath.so /usr/local/lib
	
