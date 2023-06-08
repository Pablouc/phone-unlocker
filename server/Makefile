CC = gcc
# CFLAGS = -Wall -Wextra -pedantic
CFLAGS = -pedantic


all: server

server: server.c
	$(CC) $(CFLAGS) -o server.o server.c

run: server
	./server.o

clean:
	rm -f server client
