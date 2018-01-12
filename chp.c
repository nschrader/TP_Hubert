#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "chp.h"
#include "hubert.h"

#define fatalError(x) _fatalError("[Fatal] " x);
static void _fatalError(char* errorMessage) {
  if (errno == EINTR) {
    // There was a signal so we need to exit, but it's not en error
    exit(EXIT_SUCCESS);
  }
  perror(errorMessage);
  if (errno == ENOENT) {
    fprintf(stderr, "Is %s running?\n", HUBERT_NAME);
  }
  exit(EXIT_FAILURE);
}

static MessageQueue getMessageQueue(key_t key, bool shouldCreate) {
  int flags = shouldCreate ? (IPC_CREAT | IPC_ALLWRITE) : IPC_NOFLAGS;
  MessageQueue queue = msgget(key, flags);
  if (queue == ERROR) {
    fatalError("Could not open message queue");
  }
  return queue;
}

MessageQueue createMessageQueue(key_t key) {
  return getMessageQueue(key, true);
}

MessageQueue openMessageQueue(key_t key) {
  return getMessageQueue(key, false);
}

void removeMessageQueue(MessageQueue id) {
  if (msgctl(id, IPC_RMID, NULL) == ERROR) {
    fatalError("Could not open message queue");
  }
}

Request waitForMessageQueue(MessageQueue id, Address forAddress) {
  Request request;
  if (msgrcv(id, &request, REQUEST_PAYLOAD_SIZE, forAddress, IPC_NOFLAGS) == ERROR) {
    fatalError("Cannot read from message queue");
  }
  return request;
}

void sendViaMessageQueue(MessageQueue id, Request request) {
  if (msgsnd(id, &request, REQUEST_PAYLOAD_SIZE, IPC_NOFLAGS) == ERROR) {
    fatalError("Cannot send via message queue");
  }
}
