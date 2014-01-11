CXX = clang
CXXFLAGS = -O2 -Wall -DVDBG
LDFLAGS = -lpthread
OBJS = server_io.o server_network.o server_main.o server_session.o server_synchroniser.o server_thread.o server_zombie_collector.o 

all: server

%.o:%.c
	$(CXX) $(CXXFLAGS) $< -c -o $@

server: $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o server

clean:
	rm -rf *.o server

