CC = g++
CFLAGS = -Wall -g


.PHONY: clean main

main: main.cpp 
	$(CC) $(CFLAGS) -I ./lib/ -o main main.cpp ./lib/graph.cpp
clean:
	rm -f *.o *.a *.so
	rm -f main
