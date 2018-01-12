#ifndef CHP_H
#define CHP_H

#include "entity.h"

#define CLIENT_COM    0xBEEF
#define RESTORANT_COM 0xCAFE

#define NO_ADDRESS    0x0001
#define FIRST_ADDRESS 0x0002
#define HUBERT        0xBABE

#define ERROR         -1
#define IPC_NOFLAG    0
#define IPC_ALLWRITE  0666

typedef int MessageQueue;
typedef int Address;

typedef enum {
  TALK, MENU, ORDER, OK, KO
} Command;

typedef union {
  Address address;
  Menu menu;
  Order order;
} RequestData;

typedef struct {
  long destination;
  long source;
  Command cmd;
  RequestData data;
} Request;

#define REQUEST_PAYLOAD_SIZE (sizeof(Request)-sizeof(long))
#define NO_REQUEST_DATA ((RequestData) 0)

#endif
