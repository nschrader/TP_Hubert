#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "message_queue.h"
#include "entity.h"

typedef struct {
  MessageQueue *messageQueue;
  Address this;
} Connection;

Connection* initConnection(MessageQueue* queue);
bool requestMaster(MessageQueue *queue);
Dish* requestMenu(Connection* con);
void closeConnection(Connection* con);

#endif
