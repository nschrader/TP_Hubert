CFLAGS = -g -Wall

all: message_queue.o client.o hubert.o message_queue.h protocol.h
	gcc message_queue.o client.o -o client
	gcc message_queue.o hubert.o -o hubert

clean:
	rm -rf *.o client hubert
