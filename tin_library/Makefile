all: tin_library.so

tin_library.so: tin_library.c
	gcc -shared -c tin_library.c -o tin_library.so

clean:
	rm -rf *.so
