# Makefile del proyecto
CC			= gcc
CFLAGS		= -g -Wall


all: main

main: cola.o main.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.h
color.o: color.h
dibujador.o: dibujador.h
list.o: list.h

clean:
	rm -f *o main

