#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include "message_queue.h"

#define DISH_THRESHOLD 2

RequestData* createSharedMemeory();
void readMenu(char *menuPath, RequestData *data);
void beKitchen(Dish* menu);

#endif
