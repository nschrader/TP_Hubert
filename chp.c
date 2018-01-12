#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "chp.h"
#include "hubert.h"

static MessageQueue getMessageQueue(key_t key, bool shouldCreate) {
  int flags = shouldCreate ? (IPC_CREAT | IPC_ALLWRITE) : IPC_NOFLAGS;
  MessageQueue queue = msgget(key, flags);
  if (queue == ERROR) {
    perror("[Fatal] Could not open message queue");
    if (errno == ENOENT) {
      fprintf(stderr, "Is %s running?\n", HUBERT_NAME);
    }
    exit(EXIT_FAILURE);
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
    perror("[Fatal] Could not open message queue");
    exit(EXIT_FAILURE);
  }
}
