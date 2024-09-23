CC = g++
CFLAGS = -Wall -g


.PHONY: clean main

main: clean main.cpp 
	$(CC) $(CFLAGS) -I ./lib/ -o main main.cpp ./lib/graph.cpp
clean:
	rm -f *.o *.a *.so
	rm -f main
	rm -f *.out
	rm -f *.results
