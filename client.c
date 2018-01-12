#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include "chp.h"

int main() {
  MessageQueue clientCom = openMessageQueue(CLIENT_COM);
  Request request = {HUBERT, NO_ADDRESS, TALK, NO_REQUEST_DATA};
  if (msgsnd(clientCom, &request, REQUEST_PAYLOAD_SIZE, IPC_NOFLAGS) == ERROR) {
    goto error;
  }
  if (msgrcv(clientCom, &request, REQUEST_PAYLOAD_SIZE, NO_ADDRESS, IPC_NOFLAGS) == ERROR) {
    goto error;
  }
  printf("We have no address %d\n", request.data.address);
  return EXIT_SUCCESS;

  error:
  perror("");
  return EXIT_FAILURE;
}
