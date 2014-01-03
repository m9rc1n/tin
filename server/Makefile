all: server

server_io.o: server_io.c
	gcc -c server_io.c

server_network.o: server_network.c
	gcc -c server_network.c

server_reset.o: server_reset.c
	gcc -c server_reset.c

server_main.o: server_main.c
	gcc -c server_main.c

server: server_main.o server_reset.o server_io.o server_network.o
	gcc server_main.o server_reset.o server_io.o server_network.o -o server

clean:
	rm -rf *.o server

