#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "misc.h"
#include "hubert.h"

void _fatal(char* errorMessage) {
  if (errno == EINTR) {
    // There was a signal so we need to exit, but it's not en error
    exit(EXIT_SUCCESS);
  }
  if (errno) {
    perror(errorMessage);
    if (errno == ENOENT) {
      fprintf(stderr, "Is %s running?\n", HUBERT_NAME);
    }
  } else {
    fprintf(stderr, "%s\n", errorMessage);
  }
  exit(EXIT_FAILURE);
}

void _warning(char* errorMessage) {
  if (errno) {
    perror(errorMessage);
  } else {
    fprintf(stderr, "%s\n", errorMessage);
  }
}

static void operateOnSemaphore(int semid, int sem_num, int val) {
  struct sembuf operation;
  operation.sem_num = sem_num;
  operation.sem_op = val;
  operation.sem_flg = IPC_NOFLAGS;
  if (semop(semid, &operation, 1) == ERROR) {
    fatal("Could not operate on semaphore");
  }
}

void V(int semid, int sem_num) {
  operateOnSemaphore(semid, sem_num, 1);
}

void P(int semid, int sem_num) {
  operateOnSemaphore(semid, sem_num, -1);
}

void V2(int semid, int sem_num) {
  V(semid, sem_num);
  V(semid, sem_num);
}

void P2(int semid, int sem_num) {
  P(semid, sem_num);
  P(semid, sem_num);
}
