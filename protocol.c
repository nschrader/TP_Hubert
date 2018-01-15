#include <stdlib.h>

#include "protocol.h"
#include "message_queue.h"

Connection* initConnection(MessageQueue* queue) {
  Request requestOut = {HUBERT_ADDR, NO_ADDR, TALK, NO_REQUEST_DATA};
  sendViaMessageQueue(queue, &requestOut);

  Request* requestIn = waitForMessageQueue(queue, NO_ADDR);
  Connection* con = malloc(sizeof(Connection));
  con->this = requestIn->data.address;

  free(requestIn);
  return con;
}
