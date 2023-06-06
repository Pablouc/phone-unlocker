$CC = gcc
$RM = rm
$AR = r
$PWD = $(shell pwd)

COMPILER_FLAGS += -Werror -I./include

all: ardlib compile_static

ardlib: ardlib.c
	$(CC) $(COMPILER_FLAGS) -c ardlib.c $(LINKER_FLAGS)

compile_static: ardlib
	$(AR) rcs libardlib.a ardlib.o
	rm -rf *.o

test: compile_static
	$(CC) $(COMPILER_FLAGS) -o test.o test/test_ardlib.c -L$(PWD) -lardlib
	./test.o

clean: 
	$(RM) -rf *.o *.a
