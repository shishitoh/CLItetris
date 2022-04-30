CXX = gcc
CFLAGS = -lncurses -Wall -std=c11
SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=.o)

CLItetris: $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^

.c.o:
	$(CXX) $(CFLAGS) -c $^

clean:
	rm -rf *.o
