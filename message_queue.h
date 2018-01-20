#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <sys/types.h>
#include <stdbool.h>

#include "entity.h"
#include "carrier.h"

#define CLIENT_COM    0xBEEF
#define RESTAURANT_COM 0xCAFE

#define NO_ADDR       0x0001
#define FIRST_ADDR    0x0002
#define HUBERT_ADDR   0xBABE
#define FALLBACK_ADDR 0xDEAD

#define IPC_NOFLAGS   0
#define IPC_ALLWRITE  0666

#define ERROR         -1
#define NO_ERRNO      0
#define MSGMAX        8192 //Let's hope that it's the POSIX default value

#define GCC_ATTRIBUTE_PACKED __attribute__((__packed__))

typedef struct {
  int msqid;
  int semid;
} MessageQueue;

typedef long Address;

typedef enum {
  MASTER, TALK, MENU, ORDER, OK, KO, BYE
} Command;

#define REQUEST_NO_PAYLOAD (sizeof(Address)+sizeof(Command))
#define REQUEST_DATA_N_MENUS ((MSGMAX-REQUEST_NO_PAYLOAD)/sizeof(Dish))
#define REQUEST_DATA_N_ORDERS  ((MSGMAX-REQUEST_NO_PAYLOAD)/sizeof(int))

typedef union {
  bool senderIsMaster;
  Address address;
  Carrier carrier;
  Dish menu[REQUEST_DATA_N_MENUS];
  Order order[REQUEST_DATA_N_ORDERS];
} RequestData;

typedef struct GCC_ATTRIBUTE_PACKED {
  Address destination;
  Address source;
  Command cmd;
  RequestData data;
} Request;

#define REQUEST_CAPACITY (sizeof(Request)-sizeof(Address))
#define REQUEST_PAYLOAD(x, n) (REQUEST_NO_PAYLOAD+sizeof(x)*n)
#define NO_REQUEST_DATA ((RequestData) 0L)

MessageQueue* createMessageQueue(key_t key);
MessageQueue* openMessageQueue(key_t key);
size_t getPayloadSizeFrom(Request* request);
Request* getFromMessageQueue(MessageQueue* queue, Address forAddress);
Request* waitForMessageQueue(MessageQueue* queue, Address forAddress);
void sendViaMessageQueue(MessageQueue* id, Request* request);
void removeMessageQueue(MessageQueue* id);

#endif
