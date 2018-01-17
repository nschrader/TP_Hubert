#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "protocol.h"
#include "message_queue.h"

Connection* initConnection(MessageQueue* queue) {
  Request requestOut = {HUBERT_ADDR, NO_ADDR, TALK, NO_REQUEST_DATA};
  sendViaMessageQueue(queue, &requestOut);

  Request* requestIn = waitForMessageQueue(queue, NO_ADDR);
  Connection* con = malloc(sizeof(Connection));
  con->this = requestIn->data.address;
  con->messageQueue = queue;

  free(requestIn);
  return con;
}

bool requestMaster(MessageQueue *queue) {
  RequestData data = { .senderIsMaster = false };
  Request request = {HUBERT_ADDR, FALLBACK_ADDR, MASTER, data};
  sendViaMessageQueue(queue, &request);
  usleep(500);
  Request* requestAnswer = getFromMessageQueue(queue, FALLBACK_ADDR);
  if (requestAnswer == NULL) {
    goto isMaster;
  } else {
    bool interceptedMasterCmd = (requestAnswer->cmd == MASTER);
    bool senderWasMaster = requestAnswer->data.senderIsMaster;
    bool thisInstanceIsMaster = interceptedMasterCmd && !senderWasMaster;
    free(requestAnswer);
    if (thisInstanceIsMaster) {
      goto isMaster;
    } else {
      goto isntMaster;
    }
  }

  isMaster:
  while(getFromMessageQueue(queue, HUBERT_ADDR) != NULL);
  while(getFromMessageQueue(queue, FALLBACK_ADDR) != NULL);
  return true;

  isntMaster:
  return false;
}

void closeConnection(Connection* con) {
  Request requestOut = {HUBERT_ADDR, con->this, BYE, NO_REQUEST_DATA};
  sendViaMessageQueue(con->messageQueue, &requestOut);
  free(con->messageQueue);
  free(con);
}
