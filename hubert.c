#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include "message_queue.h"
#include "protocol.h"
#include "misc.h"

static Connection* clientCom = NULL;
static Address addressCounter = FIRST_ADDR;

void removeQueuesHandler() {
  if (clientCom != NULL) {
    shutdownConnection(clientCom);
    clientCom = NULL;
  }
}

static void removeQueuesOnExit() {
  signal(SIGTERM, &removeQueuesHandler);
  signal(SIGINT, &removeQueuesHandler);
  atexit(&removeQueuesHandler);
}

static void openQueues() {
  clientCom = bootstrapConnection(CLIENT_COM);
}

static void assignNewAddress(Connection* con) {
  RequestData data = { .address = addressCounter++ };
  Request requestOut = {NO_ADDR, con->this, TALK, data};
  sendViaMessageQueue(con->messageQueue, &requestOut);
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

static void getMenu() {
  Dish menu[4] = {
    { .name = "TexMex", .price = 149 },
    { .name = "Burger", .price = 369},
    { .name = "Kebab", .price = 279},
    { 0 }
  };
  sendMenu(clientCom, menu, 2);
}

int main() {
  openQueues();
  checkIfSingleton();
  removeQueuesOnExit();

  while (true) {
    Request* requestIn = waitForMessageQueue(clientCom->messageQueue, HUBERT_ADDR);
    switch (requestIn->cmd) {
      case MASTER:
        printf("Notifying\n");
        sendMaster(clientCom);
      case TALK:
        assignNewAddress(clientCom);
        break;
      case MENU:
        printf("Looking for menu\n");
        getMenu();
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
