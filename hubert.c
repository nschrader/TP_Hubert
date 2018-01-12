#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include "chp.h"

static MessageQueue clientCom = ERROR;
static Address addressCounter = FIRST_ADDRESS;

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
    Request request;
    if (msgrcv(clientCom, &request, REQUEST_PAYLOAD_SIZE, HUBERT, IPC_NOFLAGS) == ERROR) {
      goto error;
    }
    printf("Got request no %d form %d\n", request.cmd, request.source);
    RequestData data;
    data.address = addressCounter++;
    Request request2 = {NO_ADDRESS, HUBERT, TALK, data};
    if (msgsnd(clientCom, &request2, REQUEST_PAYLOAD_SIZE, IPC_NOFLAGS) == ERROR) {
      goto error;
    }
  }
  return EXIT_SUCCESS;

  error:
  perror("123");
  return EXIT_FAILURE;
}
