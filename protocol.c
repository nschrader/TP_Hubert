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

bool requestMaster(MessageQueue *queue) {
  RequestData data = { .senderIsMaster = false };
  Request request = {HUBERT_ADDR, FALLBACK_ADDR, MASTER, data};
  sendViaMessageQueue(queue, &request);
  usleep(500);
  Request* requestAnswer = getFromMessageQueue(queue, FALLBACK_ADDR);
  if (isThisInstanceMaster(requestAnswer)) {
    while(getFromMessageQueue(queue, HUBERT_ADDR) != NULL);
    while(getFromMessageQueue(queue, FALLBACK_ADDR) != NULL);
    return true;
  } else {
    return false;
  }
}

void closeConnection(Connection* con) {
  Request requestOut = {HUBERT_ADDR, con->this, BYE, NO_REQUEST_DATA};
  sendViaMessageQueue(con->messageQueue, &requestOut);
  free(con->messageQueue);
  free(con);
}
