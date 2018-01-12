#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "chp.h"

int main() {
  int addressCounter = FIRST_ADDRESS;
  MessageQueue clientCom = msgget(CLIENT_COM, IPC_CREAT | IPC_ALLWRITE);
  if (clientCom == ERROR) {
    goto error;
  }

  while (true) {
    Request request;
    if (msgrcv(clientCom, &request, REQUEST_PAYLOAD_SIZE, HUBERT, IPC_NOFLAG) == ERROR) {
      goto error;
    }
    printf("Got request no %d form %d\n", request.cmd, request.source);
    RequestData data;
    data.address = addressCounter++;
    Request request2 = {NO_ADDRESS, HUBERT, TALK, data};
    if (msgsnd(clientCom, &request2, REQUEST_PAYLOAD_SIZE, IPC_NOFLAG) == ERROR) {
      goto error;
    }
  }
  return EXIT_SUCCESS;

  error:
  perror("");
  return EXIT_FAILURE;
}
