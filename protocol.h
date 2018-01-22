/**
 * @file
 * An abstraction for the protocol to be used with @ref MessageQueue
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "message_queue.h"
#include "entity.h"

/**
 * @struct Connection
 * A representation for a connection via a @ref MessageQueue
 * @var Connection::messageQueue
 * A message queue abstraction
 * @var Connection::this
 * Address to send from et to recieve for
 */
typedef struct {
  MessageQueue *messageQueue;
  Address this;
} Connection;

/**
 * Create a message queue and be Hubert
 * @param key
 * A message queue key
 */
Connection* bootstrapConnection(key_t key);

/**
 * Open a message queue and request an address from Hubert
 * @param key
 * A message queue key
 */
Connection* initConnection(key_t key);

/**
 * Assign a new address to the requesting ressource
 * @param con
 * The message queue to send via
 * @param newAddress
 * The address to assign
 */
void handshakeConnection(Connection* con, Address newAddress);

/**
 * Ask for being communication master (ie hubert)
 * @param con
 * The message queue to send via
 */
bool requestMaster(Connection* con);

/**
 * Tell the requesting ressource that this instance is already master
 * @param con
 * The message queue to send via
 */
void sendMaster(Connection* con);

/**
 * %Request a menu from a ressource (ie hubert or restaurant)
 * @param con
 * The message queue to send via
 * @param forAddress
 * The address of the ressource
 * @returns
 * A menu in form of an "Null-Byte" terminated array
 */
Dish* requestMenu(Connection* con, Address forAddress);

/**
 * Send a menu to the ressource (ie hubert or client)
 * @param con
 * The message queue to send via
 * @param menu
 * A "Null-Byte" terminated array to send
 * @param forAddress
 * The address of the requesting ressource
 */
void sendMenu(Connection* con, Dish* menu, Address forAddress);

/**
 * %Request an order from a ressource (ie hubert or restaurant)
 * @param con
 * The message queue to send via
 * @param orders
 * A "Null-Byte" terminated array
 * @param forAddress
 * The address of the ressource
 * @returns
 * A semaphore sem_num to you have to ask access to
 */
Carrier requestOrder(Connection* con, Order* orders, Address forAddress);

/**
 * Send an order to the ressource (ie hubert or client)
 * @param con
 * The message queue to send via
 * @param carrier
 * A semaphore sem_num you are giving access to
 * @param forAddress
 * The address of the requesting ressource
 */
void sendOrder(Connection* con, Carrier carrier, Address forAddress);

/**
 * Unregister from master
 * @param con
 * The message queue to close
 */
void closeConnection(Connection* con);

/**
 * Unregister all ressources and remove message queue
 * @param con
 * The message queue to remove
 */
void shutdownConnection(Connection* con);

#endif
