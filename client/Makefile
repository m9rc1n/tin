CXX = clang
CXXFLAGS = -O2 -Wall 
LDFLAGS = 
OBJS = client_io.o client_network.o client_main.o

all: client

%.o:%.c
	$(CXX) $(CXXFLAGS) $< -c -o $@

client: $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) ../tin_library/tin_library.so -o client

clean:
	rm -rf *.o client

