#include <stdlib.h>

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

/*void requestMaster(MessageQueue *queue) {
  RequestData data = { .isMaster = false };
  Request request = {HUBERT_ADDR, FALLBACK_ADDR, MASTER, data};
  sendViaMessageQueue(queue. &request);
  usleep(500);
  Request requestAnswer = getFromMessageQueue(queue, FALLBACK_ADDR);
  if answer
}*/

void closeConnection(Connection* con) {
  Request requestOut = {HUBERT_ADDR, con->this, BYE, NO_REQUEST_DATA};
  sendViaMessageQueue(con->messageQueue, &requestOut);
  free(con->messageQueue);
  free(con);
}
