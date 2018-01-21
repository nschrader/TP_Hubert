#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include "message_queue.h"
#include "protocol.h"
#include "misc.h"
#include "restaurant_list.h"

static Connection* clientCom = NULL;
static Address clientAddressPool = FIRST_ADDR;

static Connection* restaurantCom = NULL;
static Address restaurantAddressPool = FIRST_ADDR;
static Restaurant* restaurants;

static CarrierFleet carrierFleet= 0;

void removeQueuesHandler() {
  if (clientCom != NULL) {
    shutdownConnection(clientCom);
    clientCom = NULL;
  }
  if (restaurantCom != NULL) {
    shutdownConnection(restaurantCom);
    restaurantCom = NULL;
  }
  if (carrierFleet != 0) {
    removeCarrierFleet(carrierFleet);
    carrierFleet = 0;
  }
}

static void removeQueuesOnExit() {
  signal(SIGTERM, &removeQueuesHandler);
  signal(SIGINT, &removeQueuesHandler);
  atexit(&removeQueuesHandler);
}

static void openQueues() {
  clientCom = bootstrapConnection(CLIENT_COM);
  restaurantCom = bootstrapConnection(RESTORANT_COM);
  carrierFleet = createCarrierFleet();
}

static void checkIfSingleton() {
  bool isClientComMaster = requestMaster(clientCom);
  bool isRestoComMaster = requestMaster(restaurantCom);
  if(!isClientComMaster || !isRestoComMaster) {
    free(clientCom);
    clientCom = NULL;
    free(restaurantCom);
    restaurantCom = NULL;
    fatal("Another instance seems to be running!");
  }
}

void *doListenForRestaurantHandshake(void* p) {
  (void) p;
  while (true) {
    Request* requestIn = waitForMessageQueue(restaurantCom->messageQueue, FALLBACK_ADDR);
    switch (requestIn->cmd) {
      case MASTER:
        sendMaster(restaurantCom);
      case TALK:
        handshakeConnection(restaurantCom, restaurantAddressPool);
        restaurants = addRestaurant(restaurants, restaurantAddressPool++);
        info("New restaurant available");
        break;
      case BYE:
        restaurants = removeRestaurant(restaurants, requestIn->source);
        info("A restaurant ceased its service");
        break;
      default:
        warning("Unexpected command. Restaurant seems to be desynchronized!");
        break;
    }
    free(requestIn);
  }
  return NULL;
}

static void listenForRestaurantHandshake() {
  pthread_t thread;
  pthread_create(&thread, NULL, doListenForRestaurantHandshake, NULL);
  pthread_detach(thread);
}

void *doListenForClientHandshake(void* p) {
  (void) p;
  while (true) {
    Request* requestIn = waitForMessageQueue(clientCom->messageQueue, FALLBACK_ADDR);
    switch (requestIn->cmd) {
      case MASTER:
        sendMaster(restaurantCom);
      case TALK:
        handshakeConnection(clientCom, clientAddressPool++);
        info("New client connected");
        break;
      case BYE:
        info("A client disconnected");
        break;
      default:
        warning("Unexpected command. Client seems to be desynchronized!");
        break;
    }
    free(requestIn);
  }
  return NULL;
}

static void listenForClientHandshake() {
  pthread_t thread;
  pthread_create(&thread, NULL, doListenForClientHandshake, NULL);
  pthread_detach(thread);
}

static void handleOrder(Request* request) {
  Order* order = request->data.order;
  Restaurant* r = restaurants;
  bool ok = true;
  while (r != NULL) {
    ok &= requestOrder(restaurantCom, order, r->address);
    r = r->next;
  }
  Carrier carrier = (ok) ? dispatchCarrier(carrierFleet) : ERROR;
  sendOrder(clientCom, carrier, request->source);
}

static void compileMenu(Address forAddress) {
  Dish *menu = NULL;
  size_t menuN = 0;
  Restaurant* r = restaurants;
  while (r != NULL) {
    Dish* restaurantMenu = requestMenu(restaurantCom, r->address);
    size_t restaurantMenuSize = countDishes(restaurantMenu)-1;
    size_t lastMenuSize = menuN;
    menuN += restaurantMenuSize;
    menu = realloc(menu, menuN*sizeof(Dish));
    memcpy(menu+lastMenuSize, restaurantMenu, restaurantMenuSize*sizeof(Dish));
    r = r->next;
  }
  menu = realloc(menu, ++menuN*sizeof(Dish));
  memset(&menu[menuN-1], 0, sizeof(Dish));

  sendMenu(clientCom, menu, forAddress);
}

static void listenToClientCom() {
  while (true) {
    Request* requestIn = waitForMessageQueue(clientCom->messageQueue, HUBERT_ADDR);
    switch (requestIn->cmd) {
      case MASTER:
        sendMaster(clientCom);
      case MENU:
        compileMenu(requestIn->source);
        break;
      case ORDER:
        handleOrder(requestIn);
        break;
      default:
        warning("Unkonwn command. Am I talking to a client?");
        break;
    }
    free(requestIn);
  }
}

int main() {
  openQueues();
  checkIfSingleton();
  removeQueuesOnExit();

  listenForRestaurantHandshake();
  listenForClientHandshake();
  listenToClientCom();

  return EXIT_SUCCESS;
}
