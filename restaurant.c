#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "message_queue.h"
#include "protocol.h"
#include "misc.h"
#include "kitchen.h"

static Connection *con = NULL;
static Dish* menu = NULL;
static int shmid = 0;

void sayByeSignalHandler() {
  if (con != NULL) {
    closeConnection(con);
    con = NULL;
  }
  if (shmid != 0) {
    removeSharedMemeory(shmid);
    shmid = 0;
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
  sendMenu(con, menu, source);
}

static void getOrder(Connection* con, Order* order, Address source) {
  bool everythingAvailable = true;
  for (int i = 0; i < countOrders(order)-1; i++) {
    for (int j = 0; j < countDishes(menu)-1; j++) {
      if (order[i] == menu[j].id) {
        if (menu[j].quantity < 1) {
          everythingAvailable = false;
          break;
        }
      }
    }
  }
  if (everythingAvailable) {
    for(int i = 0; i < countOrders(order)-1; i++) {
      for(int j = 0; j < countDishes(menu)-1; j++) {
        if (order[i] == menu[j].id) {
          menu[j].quantity--;
        }
      }
    }
    info("Treated an order successfully");
    sendOrder(con, true, source);
  } else {
    warning("We are out of stock. What is the kitchen doing?");
    sendOrder(con, false, source);
  }
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

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fatal("No menu file specified");
  }
  RequestData* data = createSharedMemeory(&shmid);
  menu = data->menu;
  readMenu(argv[1], data);

  if (fork() == CHILD) {
    beKitchen(menu);
  } else {
    con = initConnection(RESTORANT_COM);
    sayByeOnExit();
    listenToHubert(con);
  }

  return EXIT_SUCCESS;
}
