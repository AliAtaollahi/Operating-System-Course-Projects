CC = g++ 
CFLAGS = -std=c++11 -Werror -I includes

all: GenreCounter.out reduce_proc.out map_proc.out

GenreCounter.out: build/main.o build/utils.o build/system.o
	$(CC) $(CFLAGS) build/main.o build/utils.o build/system.o -o GenreCounter.out

reduce_proc.out: build/reduce_proc_main.o build/reduce_proc.o build/utils.o
	$(CC) $(CFLAGS) build/reduce_proc_main.o build/reduce_proc.o build/utils.o -o reduce_proc.out

map_proc.out: build/map_proc_main.o build/map_proc.o build/utils.o
	$(CC) $(CFLAGS) build/map_proc_main.o build/map_proc.o build/utils.o -o map_proc.out

build/main.o: src/main.cpp includes/utils.hpp
	$(CC) $(CFLAGS) -c src/main.cpp -o build/main.o

build/reduce_proc.o: src/reduce_proc.cpp includes/reduce_proc.hpp
	$(CC) $(CFLAGS) -c src/reduce_proc.cpp -o build/reduce_proc.o

build/map_proc.o: src/map_proc.cpp includes/map_proc.hpp
	$(CC) $(CFLAGS) -c src/map_proc.cpp -o build/map_proc.o

build/reduce_proc_main.o: src/reduce_proc_main.cpp includes/reduce_proc.hpp
	$(CC) $(CFLAGS) -c src/reduce_proc_main.cpp -o build/reduce_proc_main.o

build/map_proc_main.o: src/map_proc_main.cpp includes/utils.hpp
	$(CC) $(CFLAGS) -c src/map_proc_main.cpp -o build/map_proc_main.o

build/utils.o: src/utils.cpp includes/utils.hpp
	$(CC) $(CFLAGS) -c src/utils.cpp -o build/utils.o

build/system.o: src/system.cpp includes/system.hpp
	$(CC) $(CFLAGS) -c src/system.cpp -o build/system.o

.PHONY: clean
clean:
	rm build/*.o
	rm *.out