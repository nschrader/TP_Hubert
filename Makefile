CC			= gcc
CFLAGS	= -g -Wall -std=c99 -D_GNU_SOURCE -Wno-missing-braces #GCC Bug GCC #53119
LDFLAGS	= -lpthread

COMMON = \
	message_queue.c \
	protocol.c \
	misc.c \
	restaurant_list.c \
	entity.c
HUBERT = \
	hubert.c
RESTAURANT = \
	restaurant.c
CLIENT = \
	client.c

COMMON_H = ${COMMON:.c=.h}
COMMON_O = ${COMMON:.c=.c.o}
HUBERT_O = ${HUBERT:.c=.c.o}
RESTAURANT_O = ${RESTAURANT:.c=.c.o}
CLIENT_O = ${CLIENT:.c=.c.o}

.PHONY: 		all clean doc

all:				hubert restaurant client #doc

hubert:			${COMMON_O} ${HUBERT_O} ${HDR}
						${CC} -o $@ $^ ${LDFLAGS}

restaurant:	${COMMON_O} ${RESTAURANT_O} ${HDR}
						${CC} -o $@ $^ ${LDFLAGS}

client:			${COMMON_O} ${CLIENT_O} ${HDR}
						${CC} -o $@ $^ ${LDFLAGS}

%.c.o:			%.c ${COMMON_H}
						${CC} -c -o $@ $< ${CFLAGS}

clean:
						rm -rf *.c.o hubert restaurant client

doc:
						doxygen
