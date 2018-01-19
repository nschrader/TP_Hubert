#include <stdio.h>
#include <stdlib.h>

#include "message_queue.h"
#include "protocol.h"

static void printMenu(Dish* menu) {
  printf("Today's menu:\n");
  printf("Order code | Description | Price\n");
  printf("-----------+-------------+---------\n");
  while (!IS_END_OF_DISHES(menu)) {
    printf("%d\t   | %s\t | %.2fEUR\n", menu->id, menu->name, menu->price / 100.0);
    menu++;
  }
}

static bool isIdInMenu(Dish* menu, int id) {
  while (!IS_END_OF_DISHES(menu)) {
    if (menu->id == id) {
      return true;
    }
    menu++;
  }
  return false;
}

static RequestData readOrder(Dish* menu) {
  char* line = NULL;
  size_t lineN = 0;
  RequestData data;

  printf("\nType order codes for home delivery. Press return to enter the next one.\n");
  printf("Press return on a blank line to finish:\n");

  int idx;
  for (idx = 0; idx < REQUEST_DATA_N_ORDERS-1; idx++) {
    getline(&line, &lineN, stdin);
    if (*line == '\n') {
      goto clean;
    }

    int id = atol(line);
    if (id == 0) {
      printf("Invalid number, try again: ");
      idx--;
      continue;
    } else if (!isIdInMenu(menu, id)) {
      printf("No such order code, try again: ");
      idx--;
      continue;
    } else {
      data.order[idx] = id;
    }
  }

  clean:
  data.order[++idx] = 0;
  free(line);
  return data;
}

int main() {
  Connection* con = initConnection(CLIENT_COM);
  printf("Got address %ld\n", con->this);

  Dish* menu = requestMenu(con);
  printMenu(menu);
  RequestData data = readOrder(menu);
  printf("%d, %d, %d\n", data.order[0], data.order[1], data.order[3]);
  //requestOrder(data);

  closeConnection(con);
  return EXIT_SUCCESS;
}
