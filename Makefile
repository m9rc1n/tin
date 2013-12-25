all:
	make all -C tin_library	
	make all -C client
	make all -C server

clean:
	make clean -C tin_library	
	make clean -C client
	make clean -C server

