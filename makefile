OBJS = main.o maze.o
CC = g++ -std=c++11
DEBUG = -g
OPTI = -O2
CFLAGS = -Wall -c $(DEBUG) $(OPTI)
LFLAGS = -Wall $(DEBUG) $(OPTI)

maze : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o maze.out

main.o : main.cpp maze.h
	$(CC) $(CFLAGS) main.cpp

maze.o : maze.cpp maze.h
	$(CC) $(CFLAGS) maze.cpp

clean :
		rm -f *.o *.out
