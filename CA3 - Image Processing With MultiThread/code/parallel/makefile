COMPILER = g++
VERSION = -std=c++11
OPTION = -w
CC = ${COMPILER} ${VERSION} ${OPTION}
LINK_FLAGS = -pthread

all: clean ImageFilters

ImageFilters : readImg.cpp
	${CC} $(LINK_FLAGS) readImg.cpp -o ImageFilters.out

clean:
	rm -f ImageFilters.out
