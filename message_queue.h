#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <sys/types.h>
#include <stdbool.h>
#include "entity.h"

#define CLIENT_COM    0xBEEF
#define RESTORANT_COM 0xCAFE

#define NO_ADDR       0x0001
#define FIRST_ADDR    0x0002
#define HUBERT_ADDR   0xBABE
#define FALLBACK_ADDR 0xDEAD

#define IPC_NOFLAGS   0
#define IPC_ALLWRITE  0666

#define ERROR         -1
#define NO_ERRNO      0
#define MSGMAX        8192 //Let's hope that it's the POSIX default value

#define GCC_MAKE_AT_LEAST_MSGMAX \
  __attribute__ ((aligned (MSGMAX)))

typedef long Address;

typedef struct {
  int msqid;
  int semid;
} MessageQueue;

typedef union {
  bool senderIsMaster;
  Address address;
  Menu menu[MSGMAX/sizeof(Menu)];
  Order order[MSGMAX/sizeof(Order)];
} RequestData;

typedef enum {
  MASTER, TALK, MENU, ORDER, OK, KO, BYE
} Command;

typedef struct {
  Address destination;
  Address source;
  Command cmd;
  RequestData data;
} Request GCC_MAKE_AT_LEAST_MSGMAX;

#define REQUEST_PAYLOAD_SIZE (sizeof(Request)-sizeof(long))
#define NO_REQUEST_DATA ((RequestData) 0L)

MessageQueue* createMessageQueue(key_t key);
MessageQueue* openMessageQueue(key_t key);
void removeMessageQueue(MessageQueue* id);
Request* getFromMessageQueue(MessageQueue* queue, Address forAddress);
Request* waitForMessageQueue(MessageQueue* queue, Address forAddress);
void sendViaMessageQueue(MessageQueue* id, Request* request);

#endif
