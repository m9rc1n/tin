CXX = clang
CXXFLAGS = -O2 -Wall 
LDFLAGS = 
TESTS = session_basic session_timeouter

SRCS = $(addsuffix .c, $(TESTS))
EXEC = $(addprefix bin/, $(addsuffix .out, $(TESTS)))

all: $(EXEC)

bin/%.out:%.c
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< ../tin_library/tin_library.so -o $@

clean:
	rm -rf bin/*.out
	

