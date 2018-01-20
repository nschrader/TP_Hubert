CFLAGS = -g -Wall -std=c99 -D_GNU_SOURCE -Wno-missing-braces #GCC Bug GCC #53119

all: message_queue.o client.o hubert.o protocol.o misc.o restaurant_list.o entity.o restaurant.o message_queue.h protocol.h misc.h entity.h carrier.h restaurant_list.h
	gcc message_queue.o client.o protocol.o misc.o entity.o restaurant_list.o -o client
	gcc -lpthread message_queue.o hubert.o protocol.o misc.o entity.o restaurant_list.o -o hubert
	gcc message_queue.o restaurant.o protocol.o misc.o entity.o restaurant_list.o -o restaurant

clean:
	rm -rf *.o client hubert restaurant
