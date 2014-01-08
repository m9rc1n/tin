CXX = clang
CXXFLAGS = -O2 -Wall 
LDFLAGS = 
OBJS = main.o
EXEC = test_session

all: $(EXEC)

%.o:%.c
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) ../../tin_library/tin_library.so -o $(EXEC)

clean:
	rm -rf *.o client

