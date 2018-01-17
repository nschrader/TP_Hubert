#include <stdio.h>
#include <stdlib.h>

#include "message_queue.h"
#include "protocol.h"

int main() {
  Connection* con = initConnection(CLIENT_COM);
  printf("Got address %ld\n", con->this);

  Dish* menu = requestMenu(con);
  while (*((int*) menu) != 0) {
    printf("Today %s for %fEUR", menu->name, menu->price / 100.0);
    menu++;
  }

  closeConnection(con);

  return EXIT_SUCCESS;
}
