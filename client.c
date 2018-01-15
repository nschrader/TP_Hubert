#include <stdio.h>
#include <stdlib.h>

#include "message_queue.h"
#include "protocol.h"

int main() {
  MessageQueue* clientCom = openMessageQueue(CLIENT_COM);
  Connection* con = initConnection(clientCom);
  printf("Got address %ld\n", con->this);

  return EXIT_SUCCESS;
}
