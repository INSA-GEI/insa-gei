CC=gcc
LINK=g++
CFLAGS=-I./../../headers
CFLAGS_SRC=-I./../../headers/
OBJ= main.o
OBJ_SRC=./../../source/*.o
LIB=

all: main

main: $(OBJ) 
	$(LINK) -lcv -lcxcore -lhighgui $(OBJ) $(OBJ_SRC) -o image

main.o: main.c dimage.o
	$(CC) -g -Wall $(CFLAGS) -c main.c -o main.o

dimage.o: ./../../source/dimage.c ./../../headers/dimageshop.h ./../../headers/dtools.h  
	cd ../../source; echo "entering source"; \
	$(CC) -g -Wall $(CFLAGS_SRC)  -c *.c
	cd ..

clean:
	rm -f $(OBJ) $(OBJ_SRC) image
