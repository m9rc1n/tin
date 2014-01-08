all:
	make all -C tin_library	
	make all -C client
	make all -C server
	make all -C tests

clean:
	make clean -C tin_library	
	make clean -C client
	make clean -C server
	make clean -C tests

