CXX = clang
CXXFLAGS = -O2 -Wall 
LDFLAGS = 
TESTS = session_basic

SRCS = $(addsuffix .c, $(TESTS))
EXEC = $(addprefix bin/, $(addsuffix .out, $(TESTS)))

all: $(EXEC)

bin/%.out: $(SRCS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< ../tin_library/tin_library.so -o $@

clean:
	rm -rf bin/*.out
	

