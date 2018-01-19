#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stddef.h>
#include "entity.h"
#include "message_queue.h"

#define NB_RESTAURANTS 50
#define NB_MAX_DISHES 30
#define MAX_SIZE_MSG 100



/**********************************************/

void waitForHubert(Connection *con){
  waitForMessageQueue(con->messageQueue,HUBERT_ADDR);
}

void checkingStock(RequestData data, Connection *con){
  for (int i=0; i < sizeof(request->menu); i++){
    if(menu[i] == NULL){
      memcpy(&data, menu, sizeof(Menu) * countDishes(menu));
      Request request = {forAddress, con->this, STOCK, data};
      sendViaMessageQueue(con->messageQueue, &request);
      printf("Dish : %s is not available. Try later",menu[i]->name);
    }
  }
}




/**********************************************/

int main(){

  key_t key;
  char *shm;

  int pid = fork();
  if (pid == -1)
    fatal("Initialization error");
  if (pid == 0){     //Restaurant
    while(1){        //Infinite loop
      Connection *con = initConnection(RESTORANT_COM);
      printf("Connection established");

      waitForHubert(con);
      checkingStock(data,con);




    }
  }




}
