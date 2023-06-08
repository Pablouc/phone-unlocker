CC = gcc
# CFLAGS = -Wall -Wextra -pedantic
# CFLAGS = -pedantic 
COMPILER_FLAGS += -Werror -I../ardlib/include

all: server

server: server.c
	make -C ../ardlib
	$(CC) $(COMPILER_FLAGS) -o server.o server.c -L../ardlib -lardlib -lpthread

run: server
	./server.o

clean:
	rm -f server client
