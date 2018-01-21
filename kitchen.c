#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "message_queue.h"
#include "kitchen.h"
#include "misc.h"

RequestData* createSharedMemeory() {
  int shmid = shmget(IPC_PRIVATE, sizeof(RequestData), IPC_CREAT | IPC_ALLWRITE);
  if (shmid == ERROR) {
    goto error;
  }
  RequestData* data = shmat(shmid, NULL, IPC_NOFLAGS);
  if (data == NULL) {
    goto error;
  }
  return data;

  error:
  fatal("Cannot open shared memory");
  return NULL;
}

void readMenu(char* menuPath, RequestData *data) {
  char* line = NULL;
  size_t lineN = 0;
  FILE* menuFile = fopen(menuPath, "r");
  if (menuFile == NULL) {
    fatal("Cannot open menu file");
  }
  srandom(time(NULL));

  Dish* menu = data->menu;
  int idx;
  for (idx = 0; idx < REQUEST_DATA_N_DISHES-1; idx++) {
    ssize_t nRead = getline(&line, &lineN, menuFile);
    if (nRead != ERROR) {
      memcpy(menu[idx].name, line, NAME_BUFFER);
      menu[idx].name[nRead-1] = '\0';
      menu[idx].name[NAME_BUFFER-1] = '\0';
    } else {
      break;
    }
    if (getline(&line, &lineN, menuFile) != ERROR) {
      menu[idx].price = atoi(line);
      if (menu[idx].price == 0) {
        goto errorInvalidFormat;
      }
      menu[idx].id = random() % 10000;
      menu[idx].quantity = 0;
    } else {
      goto errorInvalidFormat;
    }
  }
  free(line);

  if (idx%2 == 0) {
    goto errorInvalidFormat;
  }
  Dish set = { 0 };
  menu[++idx] = set;
  return;

  errorInvalidFormat:
  fatal("Cannot read menu file, invalid format");
}

void beKitchen(Dish* menu) {
  while (true) {
    for (int i = 0; i < countDishes(menu)-1; i++) {
      if (menu[i].quantity <= DISH_THRESHOLD) {
        menu[i].quantity++;
        info("Noise from the kitchen: ...Cooking");
        break;
      }
    }
    sleep(10);
  }
}
