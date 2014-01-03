all: client

client_io.o: client_io.c
	gcc -c client_io.c

client_network.o: client_network.c
	gcc -c client_network.c 

client_main.o: client_main.c
	gcc -c client_main.c

client: client_main.o client_io.o client_network.o ../tin_library/tin_library.so
	gcc client_main.o client_io.o client_network.o ../tin_library/tin_library.so -o client

clean:
	rm -rf *.o client

