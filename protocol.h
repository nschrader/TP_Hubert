#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "message_queue.h"
#include "entity.h"

typedef struct {
  MessageQueue *messageQueue;
  Address this;
} Connection;

Connection* bootstrapConnection(key_t key);
Connection* initConnection(key_t key);
void handshakeConnection(Connection* con, Address newAddress);
bool requestMaster(Connection* con);
void sendMaster(Connection* con);
Dish* requestMenu(Connection* con, Address formAddress);
void sendMenu(Connection* con, Dish* menu, Address forAddress);
Carrier requestOrder(Connection* con, Order* orders, Address forAddress);
void sendOrder(Connection* con, Carrier carrier, Address forAddress);
void closeConnection(Connection* con);
void shutdownConnection(Connection* con);

#endif
