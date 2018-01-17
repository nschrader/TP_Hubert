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

static void checkIfSingleton() {
  bool isClientComMaster = requestMaster(clientCom);
  bool isRestoComMaster = true;
  if(!isClientComMaster || !isRestoComMaster) {
    free(clientCom);
    clientCom = NULL;
    //free(restoCom);
    //restCom = NULL;
    fatal("Another instance seems to be running!");
  }
}

static void notifyOnFallbackAddr(MessageQueue* queue) {
  RequestData data = { .senderIsMaster = true };
  Request requestOut = {FALLBACK_ADDR, HUBERT_ADDR, MASTER, data};
  sendViaMessageQueue(queue, &requestOut);
}

int main() {
  openQueues();
  checkIfSingleton();
  removeQueuesOnExit();

  while (true) {
    Request* requestIn = waitForMessageQueue(clientCom, HUBERT_ADDR);
    switch (requestIn->cmd) {
      case MASTER:
        printf("Notifying\n");
        notifyOnFallbackAddr(clientCom);
      case TALK:
        assignNewAddress(clientCom);
        break;
      case BYE:
        //removeResto();
        printf("Somebody said bye\n");
        break;
      default:
        warning("Got unkonwn command, dunno what to do...");
        break;
    }
    free(requestIn);
  }

  return EXIT_SUCCESS;
}
