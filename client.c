#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include "chp.h"

int main() {
  MessageQueue clientCom = openMessageQueue(CLIENT_COM);

  Request request = {HUBERT_ADDR, NO_ADDR, TALK, NO_REQUEST_DATA};
  sendViaMessageQueue(clientCom, request);

  request = waitForMessageQueue(clientCom, NO_ADDR);
  printf("We have no address %d\n", request.data.address);
  
  return EXIT_SUCCESS;
}
