CC = gcc
CFLAGS = -g -Wall

all: tp2

tp2: tp2.o TAD_grafo.o TAD_heap.o
	$(CC) $(CFLAGS) tp2.o TAD_grafo.o TAD_heap.o -o tp2.exe

tp2.o: tp2.c TAD_grafo.h TAD_heap.h
	$(CC) $(CFLAGS) -c tp2.c

TAD_grafo.o: TAD_grafo.h TAD_grafo.c
	$(CC) $(CFLAGS) -c TAD_grafo.c 

TAD_heap.o: TAD_heap.h TAD_heap.c
	$(CC) $(CFLAGS) -c TAD_heap.c 	

clean:
	rm -f tp2.exe tp2.o TAD_grafo.o TAD_heap.o
