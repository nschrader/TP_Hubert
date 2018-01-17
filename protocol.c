#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "protocol.h"
#include "message_queue.h"
#include "entity.h"

Connection* bootstrapConnection(key_t key) {
  Connection* con = malloc(sizeof(Connection));
  con->messageQueue = createMessageQueue(key);
  con->this = HUBERT_ADDR;
  return con;
}

Connection* initConnection(key_t key) {
  MessageQueue* queue = openMessageQueue(key);
  Request requestOut = {HUBERT_ADDR, NO_ADDR, TALK, NO_REQUEST_DATA};
  sendViaMessageQueue(queue, &requestOut);

  Request* requestIn = waitForMessageQueue(queue, NO_ADDR);
  Connection* con = malloc(sizeof(Connection));
  con->this = requestIn->data.address;
  con->messageQueue = queue;

  free(requestIn);
  return con;
}

static bool isThisInstanceMaster(Request* answer) {
  if (answer == NULL) {
    return true;
  } else {
    bool interceptedMasterCmd = (answer->cmd == MASTER);
    bool senderWasMaster = answer->data.senderIsMaster;
    free(answer);
    return interceptedMasterCmd && !senderWasMaster;
  }
}

bool requestMaster(Connection *con) {
  RequestData data = { .senderIsMaster = false };
  Request request = {HUBERT_ADDR, FALLBACK_ADDR, MASTER, data};
  sendViaMessageQueue(con->messageQueue, &request);
  usleep(500);
  Request* requestAnswer = getFromMessageQueue(con->messageQueue, FALLBACK_ADDR);
  if (isThisInstanceMaster(requestAnswer)) {
    while(getFromMessageQueue(con->messageQueue, HUBERT_ADDR) != NULL);
    while(getFromMessageQueue(con->messageQueue, FALLBACK_ADDR) != NULL);
    return true;
  } else {
    return false;
  }
}

void sendMaster(Connection* con) {
  RequestData data = { .senderIsMaster = true };
  Request requestOut = {FALLBACK_ADDR, con->this, MASTER, data};
  sendViaMessageQueue(con->messageQueue, &requestOut);
}

Dish* requestMenu(Connection* con) {
  Request requestOut = {HUBERT_ADDR, con->this, MENU, NO_REQUEST_DATA};
  sendViaMessageQueue(con->messageQueue, &requestOut);

  Request* requestIn = waitForMessageQueue(con->messageQueue, con->this);
  return requestIn->data.menu;
}

void sendMenu(Connection* con, Dish* menu, Address forAddress) {
  RequestData data;
  memcpy(&data, menu, sizeof(Menu) * countDishes(menu));
  Request request = {forAddress, con->this, MENU, data};
  sendViaMessageQueue(con->messageQueue, &request);
}

void closeConnection(Connection* con) {
  Request requestOut = {HUBERT_ADDR, con->this, BYE, NO_REQUEST_DATA};
  sendViaMessageQueue(con->messageQueue, &requestOut);
  free(con->messageQueue);
  free(con);
}

void shutdownConnection(Connection* con) {
  removeMessageQueue(con->messageQueue);
  free(con);
}
