#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "message_queue.h"
#include "misc.h"
#include "entity.h"
#include "hubert.h"

#define SINGLETON 1
#define ONLY_SEMAPHORE 0

static MessageQueue* getMessageQueue(key_t key, bool shouldCreate) {
  MessageQueue* queue = malloc(sizeof(MessageQueue));
  int flags = shouldCreate ? (IPC_CREAT | IPC_ALLWRITE) : IPC_NOFLAGS;
  queue->msqid = msgget(key, flags);
  if (queue->msqid == ERROR) {
    goto error;
  }
  queue->semid = semget(key, SINGLETON, flags);
  if (queue->semid == ERROR) {
    goto cleanMsg;
  }
  if (semctl(queue->semid, ONLY_SEMAPHORE, SETVAL, 1) == ERROR) {
    goto cleanSemAndMsg;
  }
  return queue;

  cleanMsg:
  msgctl(queue->msqid, IPC_RMID, NULL);
  cleanSemAndMsg:
  msgctl(queue->semid, ONLY_SEMAPHORE, IPC_RMID);
  error:
  fatal("Could not open message queue or semaphore");
  free(queue);
  return NULL;
}

MessageQueue* createMessageQueue(key_t key) {
  return getMessageQueue(key, true);
}

MessageQueue* openMessageQueue(key_t key) {
  return getMessageQueue(key, false);
}

void removeMessageQueue(MessageQueue* queue) {
  if (msgctl(queue->msqid, IPC_RMID, NULL) == ERROR) {
    goto error;
  }
  if (semctl(queue->semid, ONLY_SEMAPHORE, IPC_RMID) == ERROR) {
    goto error;
  }
  free(queue);
  return;

  error:
  free(queue);
  fatal("Could not remove message queue");
}

static Request* checkMessageQueue(MessageQueue* queue, Address forAddress, bool shouldWait) {
  Request* request = malloc(sizeof(Request));
  int flags = shouldWait ? IPC_NOFLAGS : IPC_NOWAIT;
  if (msgrcv(queue->msqid, request, REQUEST_CAPACITY, forAddress, flags) == ERROR) {
    free(request);
    if (errno == ENOMSG && !shouldWait) {
      errno = NO_ERRNO;
      return NULL;
    } else if (errno == EIDRM) {
      fatal("Lost connection to " HUBERT_NAME);
    } else {
      fatal("Cannot read from message queue");
    }
  }
  if (forAddress == NO_ADDR) {
    V(queue->semid, ONLY_SEMAPHORE);
  }
  return request;
}

Request* getFromMessageQueue(MessageQueue* queue, Address forAddress) {
  return checkMessageQueue(queue, forAddress, false);
}

Request* waitForMessageQueue(MessageQueue* queue, Address forAddress) {
  return checkMessageQueue(queue, forAddress, true);
}

size_t getPayloadSizeFrom(Request* request) {
  switch (request->cmd) {
    case MASTER: return REQUEST_PAYLOAD(bool, 1);
    case TALK: return REQUEST_PAYLOAD(Address, 1);
    case MENU: return REQUEST_PAYLOAD(Dish, countDishes(request->data.menu));
    case ORDER: return REQUEST_PAYLOAD(Order, countOrders(request->data.order));
    case BYE: return REQUEST_NO_PAYLOAD;
    default: fatal("Unknown command");
  }
  return 0;
}

void sendViaMessageQueue(MessageQueue* queue, Request* request) {
  if (request->source == NO_ADDR) {
    P(queue->semid, ONLY_SEMAPHORE);
  }
  size_t payload = getPayloadSizeFrom(request);
  if (msgsnd(queue->msqid, request, payload, IPC_NOFLAGS) == ERROR) {
    fatal("Cannot send via message queue");
  }
}
