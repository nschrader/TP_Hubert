#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "message_queue.h"
#include "entity.h"

typedef struct {
  MessageQueue *messageQueue;
  Address this;
} Connection;

Connection* initConnection(key_t key);
Connection* bootstrapConnection(key_t key);
bool requestMaster(Connection* queue);
void sendMaster(Connection* con);
Dish* requestMenu(Connection* con);
void sendMenu(Connection* con, Dish* menu, Address forAddress);
Carrier requestOrder(Connection* con, Order* orders);
void sendOrder(Connection* con, Carrier carrier, Address forAddress);
void closeConnection(Connection* con);
void shutdownConnection(Connection* con);

#endif
