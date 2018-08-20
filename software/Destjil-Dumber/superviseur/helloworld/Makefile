CC=gcc
LINK=g++
CFLAGS=-I/usr/xenomai/include
CFLAGS_SRC=-I/usr/xenomai/include
OBJ=hello.o
LIB=

all: main


main: $(OBJ)
	$(LINK) -lxenomai -lnative -L/usr/xenomai/lib $(OBJ) $(OBJ_SRC) -o hello


hello.o: hello.c
	$(CC) -g -Wall $(CFLAGS) -c hello.c -o hello.o

clean:
	rm -f $(OBJ) $(OBJ_SRC) hello