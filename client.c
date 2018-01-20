#include <stdio.h>
#include <stdlib.h>

#include "message_queue.h"
#include "protocol.h"
#include "misc.h"
#include "hubert.h"

static void printMenu(Dish* menu) {
  printf("Today's menu:\n");
  printf("Order code | Description | Price\n");
  printf("-----------+-------------+---------\n");
  while (!IS_END_OF_DISHES(menu)) {
    printf("%d\t   | %s\t | %.2fEUR\n", menu->id, menu->name, menu->price / 100.0);
    menu++;
  }
}

static bool isIdInMenu(Dish* menu, Order id) {
  while (!IS_END_OF_DISHES(menu)) {
    if (menu->id == id) {
      return true;
    }
    menu++;
  }
  return false;
}

static bool evaluateLine(Dish* menu, char* line, Order* order) {
  Order id = atol(line);
  if (id == 0) {
    printf("Invalid number, try again: ");
    return false;
  } else if (!isIdInMenu(menu, id)) {
    printf("No such order code, try again: ");
    return false;
  } else {
    *order = id;
  }
  return true;
}

static Order* readOrder(Dish* menu) {
  char* line = NULL;
  size_t lineN = 0;
  static Order order[REQUEST_DATA_N_ORDERS];

  printf("\nType order codes for home delivery. Press return to enter the next one.\n");
  printf("Press return on a blank line to finish:\n");

  int idx;
  for (idx = 0; idx < REQUEST_DATA_N_ORDERS-1; idx++) {
    getline(&line, &lineN, stdin);
    if (*line == '\n') {
      goto clean;
    }
    if (!evaluateLine(menu, line, &order[idx])) {
      idx--;
      continue;
    }
  }

  clean:
  order[++idx] = 0;
  free(line);
  return order;
}

int main() {
  Connection* con = initConnection(CLIENT_COM);

  Dish* menu = requestMenu(con, HUBERT_ADDR);
  printMenu(menu);
  Order* order = readOrder(menu);
  Carrier carrier = requestOrder(con, order);

  CarrierFleet fleet = openCarrierFleet();
  if (carrier == ERROR) {
    printf("%s is out of carriers. Your order cannot be delivered. ", HUBERT_NAME);
    printf("Please try again later!\n");
  } else {
    printf("Waiting for Carrier...\n");
    waitForCarrier(carrier, fleet);
  }

  closeConnection(con);
  return EXIT_SUCCESS;
}
