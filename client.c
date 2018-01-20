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

/********************************* PARSING TXT ************************************/

static void readMenu(FILE *file){
  char stream[NAME_BUFFER];
  if(file == NULL)
    fprintf(stderr,"Menu does not exist");
  else{
    printf("Today's menu:\n");
    printf("Order code | Description | Price\n");
    while(fgets(stream,NAME_BUFFER,file) != NULL){
      printf("%s",stream);
    }
    printf("-----------+-------------+---------\n");
  }
}

/**********************************************************************************/

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

  Dish* menu = requestMenu(con);
  printMenu(menu);
  Order* order = readOrder(menu);
  Carrier carrier = requestOrder(con, order);
  printf("Got carrier %d\n", carrier);

  closeConnection(con);
  return EXIT_SUCCESS;
}
