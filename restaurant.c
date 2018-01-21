#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "message_queue.h"
#include "protocol.h"
#include "misc.h"

Connection *con = NULL;

void sayByeSignalHandler() {
  if (con != NULL) {
    closeConnection(con);
    con = NULL;
  }
}

void sayByeExitHandler(int exitStatus, void* p) {
  (void) p;
  if (exitStatus != EXIT_FAILURE) {
    sayByeSignalHandler();
  }
}

static void sayByeOnExit() {
  signal(SIGTERM, &sayByeSignalHandler);
  signal(SIGINT, &sayByeSignalHandler);
  on_exit(&sayByeExitHandler, NULL);
}

static void getMenu(Connection* con, Address source) {
  //Get this stuff from kitchen
  Dish menu[4] = {
    { .id = 581, .name = "TexMex", .price = 149 },
    { .id = 582, .name = "Burger", .price = 369},
    { .id = 583, .name = "Kebab", .price = 279},
    { 0 }
  };
  sendMenu(con, menu, source);
}

static void getOrder(Connection* con, Order* order, Address source) {
  //Remove from kitchen, if this restaurant offers this dish
  //* If we don't send ok = true anyways
  //* If we do and we have more than 0 dishes left, send ok = true
  //* If we do and we have only 0 dishes left, send ok = true
  printf("Got orders: %d, %d, %d\n", order[0], order[1], order[3]);
  sendOrder(con, true, source);
}

static void listenToHubert(Connection* con) {
  while (true) {
    Request* requestIn = waitForMessageQueue(con->messageQueue, con->this);
    switch (requestIn->cmd) {
      case MENU:
        getMenu(con, requestIn->source);
        break;
      case ORDER:
        getOrder(con, requestIn->data.order, requestIn->source);
        break;
      default:
        warning("Got unkonwn command, dunno what to do...");
        break;
    }
    free(requestIn);
  }
}

int main() {
  con = initConnection(RESTORANT_COM);
  sayByeOnExit();
  listenToHubert(con);
}
