CFLAGS = -g -Wall -Wno-missing-braces #GCC Bug GCC #53119

all: message_queue.o client.o hubert.o protocol.o misc.o entity.o message_queue.h protocol.h misc.h entity.h
	gcc message_queue.o client.o protocol.o misc.o entity.o -o client
	gcc message_queue.o hubert.o protocol.o misc.o entity.o -o hubert

clean:
	rm -rf *.o client hubert
