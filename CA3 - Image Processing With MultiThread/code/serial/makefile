COMPILER = g++
VERSION = -std=c++11
OPTION = -w
CC = ${COMPILER} ${VERSION} ${OPTION}


all: clean ImageFilters

ImageFilters : readImg.cpp
	${CC} readImg.cpp -o ImageFilters.out

clean:
	rm -f ImageFilters.out
