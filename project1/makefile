# Makefile
CC = gcc
CFLAGS= -I. -g
DEPS = utils.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

requester: main.o utils.o 
					$(CC) -pthread -o requester main.o utils.o

clean:
			rm *.o requester
