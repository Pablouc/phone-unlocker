CC = gcc
CFLAGS = -pedantic

all: client

client: client.c
	$(CC) $(CFLAGS) -o client.o client.c -lwebsockets

run: client
	./client.o

clean:
	rm -f server client
