CC=gcc
LINK=g++

CFLAGS= -g -Wall -I./../../headers

SOURCES=main.c $(wildcard ./../../source/*.c)
OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE=evalduree

ifndef (CIBLE)
	CIBLE=toto
endif
ifeq ($(CIBLE),raspberry)
	LIB=-lopencv_core -lopencv_highgui -lopencv_imgproc
else
	LIB=-lcv -lcxcore -lhighgui
endif

all: $(OBJECTS) 
		$(LINK) $(LIB) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<;

clean:
	rm -rf $(OBJECTS)

mrproper: clean
	rm -rf $(EXECUTABLE)