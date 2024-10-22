all: shreg reader writer testmem_child \
	mapintsfile genintsfile2 reader writer  

CFLAGS= -g -Wall 

shreg: shreg.c
	gcc -o shreg $(CFLAGS) shreg.c


mapintsfile: mapintsfile.c
	gcc -o mapintsfile $(CFLAGS) mapintsfile.c 


testmem_child: testmem_child.c
	gcc -o testmem_child $(CFLAGS) testmem_child.c 


genintsfile2: genintsfile2.c
	gcc -o genintsfile2 $(CFLAGS) genintsfile2.c 	

	
reader: reader.c
	gcc -o reader $(CFLAGS) reader.c

writer: writer.c
	gcc -o writer $(CFLAGS) writer.c
	
	
clean:
	rm -rf reader writer testmem_child  shreg \
	mapintsfile genintsfile2 reader writer 

