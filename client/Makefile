CXX = clang
CXXFLAGS = -O2 -Wall
LDFLAGS = -lncurses
OBJS = ncurses-menu.o ncurses-readstring.o clientGUI.o

all: client

%.o:%.c
	$(CXX) $(CXXFLAGS) $< -c -o $@

client: $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) ../tin_library/tin_library.so -o client

clean:
	rm -rf *.o client

