/**
 * @file
 * A IPC message queue abstraction
 */

#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <sys/types.h>
#include <stdbool.h>

#include "entity.h"
#include "carrier.h"

#define CLIENT_COM    0xBEEF //!< Message queue key for client communication
#define RESTORANT_COM 0xCAFE //!< Message queue key for restaurant communication

#define NO_ADDR       0x0001 //!< Some kind of blocking broadcast address
#define FIRST_ADDR    0x0002 //!< First address to assign dynamically
#define HUBERT_ADDR   0xBABE //!< Hubert's address
#define FALLBACK_ADDR 0xDEAD //!< Hubert's fallback address

#define MSGMAX        8192   //!< Let's hope that's the POSIX default value !

/**
 * A convenience macro
 */
#define GCC_ATTRIBUTE_PACKED __attribute__((__packed__))

/**
 * @struct MessageQueue
 * A message queue abstraction
 * @var MessageQueue::msqid
 * The IPC msqid
 * @var MessageQueue::semid
 * The IPC semid
 */
typedef struct {
  int msqid;
  int semid;
} MessageQueue;

/**
 * An abstraction of the IPC mtype field
 */
typedef long Address;

/**
 * @enum Command
 * Definition of the protocol we are using
 * @var Command::MASTER
 * Ask for being communication master (ie hubert)
 * @var Command::TALK
 * Register and get address from master
 * @var Command::MENU
 * Retrieve menu from restaurant
 * @var Command::ORDER
 * Submit order to restaurant and get carrier
 * @var Command::BYE
 * Unregister from master
 */
typedef enum {
  MASTER, TALK, MENU, ORDER, BYE
} Command;

//! Size of an request without payload
#define REQUEST_NO_PAYLOAD (sizeof(Address)+sizeof(Command))
//! Quantiy of @Dish we can stock in @RequestData
#define REQUEST_DATA_N_DISHES ((MSGMAX-REQUEST_NO_PAYLOAD)/sizeof(Dish))
//! Quantiy of @Order we can stock in @RequestData
#define REQUEST_DATA_N_ORDERS  ((MSGMAX-REQUEST_NO_PAYLOAD)/sizeof(int))

/**
 * @union RequestData
 * Depending on the command we are going to use on of the follwing fields
 * @var RequestData::senderIsMaster
 * @ref Command::MASTER
 * @var RequestData::address
 * @ref Command::TALK
 * @var RequestData::carrier
 * @ref Command::ORDER
 * @var RequestData::menu
 * @ref Command::MENU
 * @var RequestData::order
 * @ref Command::ORDER
 */
typedef union {
  bool senderIsMaster;
  Address address;
  Carrier carrier;
  Dish menu[REQUEST_DATA_N_DISHES];
  Order order[REQUEST_DATA_N_ORDERS];
} RequestData;

/**
 * @struct Request
 * A request to be send via a message queue
 * @var Request::destination
 * @var Request::source
 * @var Request::cmd
 * @var Request::data
 */
typedef struct GCC_ATTRIBUTE_PACKED {
  Address destination;
  Address source;
  Command cmd;
  RequestData data;
} Request;

//! Maximal size of a request
#define REQUEST_CAPACITY (sizeof(Request)-sizeof(Address))
//! Size of a request with certain payload
#define REQUEST_PAYLOAD(x, n) (REQUEST_NO_PAYLOAD+sizeof(x)*n)
//! Convenience macro to leave @ref Request::data blank
#define NO_REQUEST_DATA ((RequestData) 0L)

/**
 * Create a message queue if it does not exist yet
 * @param
 * A message queue key
 * @returns
 * A message queue abstraction
 */
MessageQueue* createMessageQueue(key_t key);

/**
 * Open a message queue that does exist
 * @param
 * A message queue key
 * @returns
 * A message queue abstraction
 */
MessageQueue* openMessageQueue(key_t key);

/**
 * Calculate payload size for given request.
 * Considers @ref Request::cmd and @ref RequestData.
 * @returns
 * Request size that needs to be send
 */
size_t getPayloadSizeFrom(Request* request);

/**
 * Make a non blocking attempt to read from message queue
 */
Request* getFromMessageQueue(MessageQueue* queue, Address forAddress);
Request* waitForMessageQueue(MessageQueue* queue, Address forAddress);
void sendViaMessageQueue(MessageQueue* id, Request* request);
void removeMessageQueue(MessageQueue* id);

#endif
