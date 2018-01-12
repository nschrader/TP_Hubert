#ifndef CHP_H
#define CHP_H

#include <sys/types.h>
#include "entity.h"

#define CLIENT_COM    0xBEEF
#define RESTORANT_COM 0xCAFE

#define NO_ADDR       0x0001
#define FIRST_ADDR    0x0002
#define HUBERT_ADDR   0xBABE

#define ERROR         -1
#define IPC_NOFLAGS   0
#define IPC_ALLWRITE  0666

typedef int MessageQueue;
typedef long Address;

typedef enum {
  TALK, MENU, ORDER, OK, KO
} Command;

typedef union {
  Address address;
  Menu menu;
  Order order;
} RequestData;

typedef struct {
  Address destination;
  Address source;
  Command cmd;
  RequestData data;
} Request;

#define REQUEST_PAYLOAD_SIZE (sizeof(Request)-sizeof(long))
#define NO_REQUEST_DATA ((RequestData) 0L)

MessageQueue createMessageQueue(key_t key);
MessageQueue openMessageQueue(key_t key);
void removeMessageQueue(MessageQueue id);
Request waitForMessageQueue(MessageQueue id, Address forAddress);
void sendViaMessageQueue(MessageQueue id, Request request);

#endif
