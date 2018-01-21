/**
 * @file
 * Kitchen is part (a child process) of restaurant
 */

#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include "message_queue.h"

/**
 * We want to have always more dishes in stock than this
 */
#define DISH_THRESHOLD 2

/**
 * Create shared memory to be used with kitchen
 * @returns
 * The attched shared memory
 */
RequestData* createSharedMemeory();

/**
 * Read menu from file to (shared) memory
 * @param menuPath
 * The file to read from
 * @param data
 * The memory area to write to
 */
void readMenu(char *menuPath, RequestData *data);

/**
 * Do whatever the kitchen should be doing
 * @param stock
 * The stock to take care of
 */
void beKitchen(Dish* stock);

#endif
