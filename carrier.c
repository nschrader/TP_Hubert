#include "carrier.h"
#include "misc.h"
#include <semaphore.h>
#include <time.h>

#define random_time(a,b) ((rand()/(double)RAND_MAX) * (b-a) + a)
#define NB_CARRIERS 15

int initCarrier(){
  Carrier carrier;
  return sem_init(carrier,CARRIER_SEM,NB_CARRIERS);
}

void carrierRide(float time){
  clock_t end = clock()+(time*CLOCKS_PER_SEC);
  while(clock()<end);
}

void requestCarrier(Connection *con, Order *orders,Carrier carrier){
    carrierRide(random_time(10.0,20.0));
    printf("Command has been delivered ! See you next time !");
}

int isCarrierFree(Carrier carrier){
  for(int i = 0, i < NB_CARRIERS, i++){
    if(semctl(carrier,GETVAL,i) == 0)
      printf("Carrier n° %d is available",i);
  }
}
