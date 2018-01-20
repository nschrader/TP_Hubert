#include <stdio.h>
#include <stdlib.h>

#include "message_queue.h"
#include "protocol.h"
#include "misc.h"

static void getMenu(Connection* con, Address source) {
  Dish menu[4] = {
    { .id = 581, .name = "TexMex", .price = 149 },
    { .id = 582, .name = "Burger", .price = 369},
    { .id = 583, .name = "Kebab", .price = 279},
    { 0 }
  };
  sendMenu(con, menu, source);
}

static void listenToHubert(Connection* con) {
  while (true) {
    Request* requestIn = waitForMessageQueue(con->messageQueue, con->this);
    switch (requestIn->cmd) {
      case MENU:
        getMenu(con, requestIn->source);
        break;
      default:
        warning("Got unkonwn command, dunno what to do...");
        break;
    }
    free(requestIn);
  }
}

int main() {
  Connection *con = initConnection(RESTORANT_COM);
  listenToHubert(con);

}
