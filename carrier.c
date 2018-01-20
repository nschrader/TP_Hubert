#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "carrier.h"
#include "misc.h"

#define random_time(a,b) ((rand()/(double)RAND_MAX) * (b-a) + a)

typedef struct {
  CarrierFleet fleet;
  Carrier carrier;
} DispatchOpts;

static int setCarriers(int semid) {
  unsigned short set[N_CARRIERS];
  for (int i = 0; i < N_CARRIERS; i++) {
    set[i] = 1;
  }
  return semctl(semid, N_CARRIERS, SETALL, set);
}

static CarrierFleet getCarrierFleet(bool shouldCreate) {
  int flags = shouldCreate ? (IPC_CREAT | IPC_ALLWRITE) : IPC_NOFLAGS;
  int semid = semget(CARRIER_SEM, N_CARRIERS, flags);
  if (semid == ERROR) {
    goto error;
  }
  if (setCarriers(semid) == ERROR) {
    goto error;
  }
  return semid;

  error:
  fatal("Could not open carrier semephores");
  return 0;
}

CarrierFleet createCarrierFleet() {
  return getCarrierFleet(true);
}

CarrierFleet openCarrierFleet() {
  return getCarrierFleet(false);
}

static Carrier getFreeCarrier(CarrierFleet fleet) {
  for (int i = 0; i < N_CARRIERS; i++) {
    int truc = semctl(fleet, i, GETVAL);
    if (truc == 1) {
      return i;
    }
  }
  return ERROR;
}

void* dispatchSpecificCarrier(void* p) {
  DispatchOpts* opts = (DispatchOpts*) p;
  sleep(random_time(5.0, 20.0));
  V2(opts->fleet, opts->carrier);
  return NULL;
}

Carrier dispatchCarrier(CarrierFleet fleet) {
  Carrier carrier = getFreeCarrier(fleet);
  if (carrier != ERROR) {
    pthread_t thread;
    DispatchOpts *opts = malloc(sizeof(DispatchOpts));
    opts->fleet = fleet;
    opts->carrier = carrier;
    pthread_create(&thread, NULL, dispatchSpecificCarrier, opts);
  }

  return carrier;
}

void waitForCarrier(Carrier carrier, CarrierFleet fleet) {
  P2(fleet, carrier);
  printf("Order has been delivered! See you next time!\n");
}

void removeCarrierFleet(CarrierFleet fleet) {
  if (semctl(fleet, N_CARRIERS, IPC_RMID) == ERROR) {
    fatal("Could not remove carrier semaphores");
  }
}
