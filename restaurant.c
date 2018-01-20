#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shmem.h"


#include "restaurant.h"
#include "message_queue.h"


void readMenu(FILE *file){
  char stream[NAME_BUFFER];
  if(file == NULL)
    fprintf(stderr,"Menu does not exist");
  else{
    printf("Today's menu:\n");
    printf("Order code | Description | Price\n");
    while(fgets(stream,NAME_BUFFER,file) != NULL){
      printf("%s",stream);
    }
    printf("-----------+-------------+---------\n");
  }
}

Restaurant* initRestaurant(int id,FILE *f){
  Restaurant* temp;
  temp = malloc(sizeof(Restaurant));
  temp->restaurant_id = id;
  temp->menu = f;
  return(temp);
}


int main(){

FILE *file = fopen("menu.txt","r");
int key = RESTAURANT_COM;
int size_mem = 500;

Restaurant* NickAndCheese = initRestaurant(1,file);
readMenu(NickAndCheese->menu);

int shm = create_shmem(key,size_mem);




free(NickAndCheese);
return(0);

}
