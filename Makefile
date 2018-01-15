CFLAGS = -g -Wall

all: chp.o client.o hubert.o
	gcc chp.o client.o -o client
	gcc chp.o hubert.o -o hubert
