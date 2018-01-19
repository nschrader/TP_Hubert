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

static void assignNewAddress() {
  RequestData data = { .address = addressCounter++ };
  Request requestOut = {NO_ADDR, HUBERT_ADDR, TALK, data};
  sendViaMessageQueue(clientCom->messageQueue, &requestOut);
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

static void getMenu(Address source) {
  Dish menu[4] = {
    { .id = 581, .name = "TexMex", .price = 149 },
    { .id = 582, .name = "Burger", .price = 369},
    { .id = 583, .name = "Kebab", .price = 279},
    { 0 }
  };
  sendMenu(clientCom, menu, source);
}

static void handleOrder(Request* request) {
  Order* order = request->data.order;
  printf("%d, %d, %d\n", order[0], order[1], order[3]);
  //do stuff with order
  //get carrier
  sendOrder(clientCom, 1, request->source);
}

int main() {
  openQueues();
  checkIfSingleton();
  removeQueuesOnExit();

  while (true) {
    Request* requestIn = waitForMessageQueue(clientCom->messageQueue, HUBERT_ADDR);
    switch (requestIn->cmd) {
      case MASTER:
        sendMaster(clientCom);
      case TALK:
        assignNewAddress();
        break;
      case MENU:
        getMenu(requestIn->source);
        break;
      case ORDER:
        printf("Recieving order\n");
        handleOrder(requestIn);
        break;
      case BYE:
        break;
      default:
        warning("Got unkonwn command, dunno what to do...");
        break;
    }
    free(requestIn);
  }

  return EXIT_SUCCESS;
}
