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

#define SIGNLETON 1
#define ONLY_SEMAPHORE 0

static MessageQueue* getMessageQueue(key_t key, bool shouldCreate) {
  MessageQueue* queue = malloc(sizeof(MessageQueue));
  int flags = shouldCreate ? (IPC_CREAT | IPC_ALLWRITE) : IPC_NOFLAGS;
  queue->msqid = msgget(key, flags);
  if (queue->msqid == ERROR) {
    goto error;
  }
  queue->semid = semget(key, SIGNLETON, flags);
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

static void operateOnSemaphore(int semid, int val) {
  struct sembuf operation;
  operation.sem_num = ONLY_SEMAPHORE;
  operation.sem_op = val;
  operation.sem_flg = IPC_NOFLAGS;
  if (semop(semid, &operation, 1) == ERROR) {
    fatal("Could not operate on semaphore");
  }
}

static void V(int semid) {
  operateOnSemaphore(semid, 1);
}

static void P(int semid) {
  operateOnSemaphore(semid, -1);
}

static Request* checkMessageQueue(MessageQueue* queue, Address forAddress, bool shouldWait) {
  Request* request = malloc(sizeof(Request));
  int flags = shouldWait ? IPC_NOFLAGS : IPC_NOWAIT;
  if (msgrcv(queue->msqid, request, REQUEST_CAPACITY, forAddress, flags) == ERROR) {
    free(request);
    if (errno == ENOMSG && !shouldWait) {
      errno = NO_ERRNO;
      return NULL;
    } else {
      fatal("Cannot read from message queue");
    }
  }
  if (forAddress == NO_ADDR) {
    V(queue->semid);
  }
  return request;
}

Request* getFromMessageQueue(MessageQueue* queue, Address forAddress) {
  return checkMessageQueue(queue, forAddress, false);
}

Request* waitForMessageQueue(MessageQueue* queue, Address forAddress) {
  return checkMessageQueue(queue, forAddress, true);
}

size_t getPayloadSizeFromCommand(Command cmd) {
  switch (cmd) {
    case MASTER: return REQUEST_PAYLOAD(bool);
    case TALK: return REQUEST_PAYLOAD(Address);
    case BYE: return REQUEST_NO_PAYLOAD;
    default: fatal("Not implemented command");
  }
  return 0;
}

void sendViaMessageQueue(MessageQueue* queue, Request* request) {
  if (request->source == NO_ADDR) {
    P(queue->semid);
  }
  size_t payload = getPayloadSizeFromCommand(request->cmd);
  if (msgsnd(queue->msqid, request, payload, IPC_NOFLAGS) == ERROR) {
    fatal("Cannot send via message queue");
  }
}
