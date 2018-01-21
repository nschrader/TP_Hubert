#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include "message_queue.h"

#define DISH_STOCK 2
#define DISH_THRESHOLD 1

RequestData* createSharedMemeory();
void readMenu(char *menuPath, RequestData *data);
void beKitchen(Dish* menu);

#endif
