#Makefile to compile executable for the program sttyl.c, which is a light weight
#version of stty.
#date: 3/2/2017
#author: Tasuku Miura
CC  = gcc
CFLAGS = -Wall -g

sttyl: sttyl.o sttyltables.o sttylutil.o
	$(CC) $(CFLAGS) -o sttyl sttyl.o sttyltables.o sttylutil.o

sttyl.o: sttyl.c
	$(CC) $(CFLAGS) -c sttyl.c


clean:
	rm -f sttyl sttyl.o sttyltables.o sttylutil.o
