#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "message_queue.h"
#include "hubert.h"

#define SIGNLETON 1
#define ONLY_SEMAPHORE 0

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
  fatalError("Could not open message queue or semaphore");
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
  fatalError("Could not remove message queue");
}

static void operateOnSemaphore(int semid, int val) {
  struct sembuf operation;
  operation.sem_num = ONLY_SEMAPHORE;
  operation.sem_op = val;
  operation.sem_flg = IPC_NOFLAGS;
  if (semop(semid, &operation, 1) == ERROR) {
    fatalError("Could not operate on semaphore");
  }
}

static void V(int semid) {
  operateOnSemaphore(semid, 1);
}

static void P(int semid) {
  operateOnSemaphore(semid, -1);
}

Request* waitForMessageQueue(MessageQueue* queue, Address forAddress) {
  Request* request = malloc(sizeof(Request));
  if (msgrcv(queue->msqid, request, REQUEST_PAYLOAD_SIZE, forAddress, IPC_NOFLAGS) == ERROR) {
    free(request);
    fatalError("Cannot read from message queue");
  }
  if (forAddress == NO_ADDR) {
    V(queue->semid);
  }
  return request;
}

void sendViaMessageQueue(MessageQueue* queue, Request* request) {
  if (request->source == NO_ADDR) {
    P(queue->semid);
  }
  if (msgsnd(queue->msqid, request, REQUEST_PAYLOAD_SIZE, IPC_NOFLAGS) == ERROR) {
    fatalError("Cannot send via message queue");
  }
}
