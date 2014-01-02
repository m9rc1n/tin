all: client

io.o: io.c
	gcc -c io.c

network.o: network.c
	gcc -c network.c 

main.o: main.c
	gcc -c main.c

client: main.o io.o network.o ../tin_library/tin_library.so
	gcc main.o io.o network.o ../tin_library/tin_library.so -o client

clean:
	rm -rf *o client

