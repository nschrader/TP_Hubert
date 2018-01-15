#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "message_queue.h"

typedef struct {
  MessageQueue *messageQueue;
  Address this;
} Connection;

Connection* initConnection(MessageQueue* queue);
void closeConnection(Connection* con);

#endif
