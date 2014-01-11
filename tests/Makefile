CXX = clang
CXXFLAGS = -O2 -Wall -DVDBG
LDFLAGS = 
TESTS = file_reader file_writer file_opener session_basic session_timeouter

SRCS = $(addsuffix .c, $(TESTS))
EXEC = $(addprefix bin/, $(addsuffix .out, $(TESTS)))

all: $(EXEC)

bin/%.out:%.c
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< ../tin_library/tin_library.so -o $@

clean:
	rm -rf bin/*.out
	

