#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include "chp.h"

static MessageQueue clientCom = ERROR;
static Address addressCounter = FIRST_ADDR;

void removeQueuesHandler() {
  if (clientCom != ERROR) {
    removeMessageQueue(clientCom);
    clientCom = ERROR;
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

int main() {
  openQueues();
  removeQueuesOnExit();

  while (true) {
    Request requestIn = waitForMessageQueue(clientCom, HUBERT_ADDR);
    printf("Got request no %ld form %ld\n", requestIn.cmd, requestIn.source);

    RequestData data = { .address = addressCounter++ };
    Request requestOut = {NO_ADDR, HUBERT_ADDR, TALK, data};
    sendViaMessageQueue(clientCom, requestOut);
  }

  return EXIT_SUCCESS;
}
