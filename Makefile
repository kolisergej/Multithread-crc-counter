CC=g++
CFLAGS=-c -Wall -O2
LDFLAGS=-std=c++0x -pthread

all: veeamTest

veeamTest: main.o Common.o
	$(CC) Common.o main.o -o veeamTest

Common.o: Common.h Common.cpp
	$(CC) $(CFLAGS) Common.cpp

main.o: Common.h main.cpp
	$(CC) $(LDFLAGS) $(CFLAGS) main.cpp

clean: 
	rm -rf veeamTest *.o