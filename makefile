# Makefile del proyecto
CC			= gcc
CFLAGS		= -g -Wall


all: main

main: cola.o main.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread -lrt

main.o: main.h
cola.o: cola.h

clean:
	rm -f *o main

