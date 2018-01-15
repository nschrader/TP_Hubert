#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include "message_queue.h"
#include "protocol.h"
#include "misc.h"

static MessageQueue* clientCom = NULL;
static Address addressCounter = FIRST_ADDR;

void removeQueuesHandler() {
  if (clientCom != NULL) {
    removeMessageQueue(clientCom);
    clientCom = NULL;
  }
}

static void removeQueuesOnExit() {
  signal(SIGTERM, &removeQueuesHandler);
  signal(SIGINT, &removeQueuesHandler);
  atexit(&removeQueuesHandler);
}

static void openQueues() {
  clientCom = createMessageQueue(CLIENT_COM);
}

static void assignNewAddress(MessageQueue* queue) {
  RequestData data = { .address = addressCounter++ };
  Request requestOut = {NO_ADDR, HUBERT_ADDR, TALK, data};
  sendViaMessageQueue(clientCom, &requestOut);
}

int main() {
  openQueues();
  removeQueuesOnExit();

  recover:
  while (true) {
    Request* requestIn = waitForMessageQueue(clientCom, HUBERT_ADDR);
    switch (requestIn->cmd) {
      case TALK:
        assignNewAddress(clientCom);
        break;
      default:
        warning("Got unkonwn command, dunno what to do..."); 
        free(requestIn);
        goto recover;
    }
  }

  return EXIT_SUCCESS;
}
