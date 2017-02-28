OBJS = main.o maze.o
CC = g++ -std=c++11
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

maze : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o maze.out

main.o : main.cpp maze.h
	$(CC) $(CFLAGS) main.cpp

maze.o : maze.cpp maze.h
	$(CC) $(CFLAGS) maze.cpp

clean :
		rm -f *.o *.out
