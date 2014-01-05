CXX = clang
CXXFLAGS = -O2 -Wall 
LDFLAGS = -lpthread
OBJS = server_io.o server_network.o server_main.o server_synchroniser.o server_thread.o

all: server

%.o:%.c
	$(CXX) $(CXXFLAGS) $< -c -o $@

server: $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o server

clean:
	rm -rf *.o server

