CXX=clang

all: tin_library.so

tin_library.so: tin_library.c tin_library.h
	$(CXX) -shared -c tin_library.c -o tin_library.so

clean:
	rm -rf *.so
