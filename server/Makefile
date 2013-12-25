all: server

io.o: io.c
	gcc -c io.c

network.o: network.c
	gcc -c network.c

reset.o: reset.c
	gcc -c reset.c

main.o: main.c
	gcc -c main.c

server: main.o reset.o io.o network.o
	gcc main.o reset.o io.o network.o -o server

clean:
	rm -rf *o server

