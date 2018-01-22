#ifndef RESTAURANT_LIST_H
#define RESTAURANT_LIST_H

#include "message_queue.h"

/**
 *@struct Restaurant
 *Represents a restaurants
 *@var address::adress of the restaurant
 *@var *next::pointer to next restaurant
 */

typedef struct _Restaurant {
  Address address;
  struct _Restaurant *next;
} Restaurant;

/**
 *Adds a restaurant to hUBERt platform
 *@param restaurants
 *Restaurant to add
 *@param address
 *Address of the restaurant to add
 */

Restaurant* addRestaurant(Restaurant* restaurants, Address address);

/**
 *Removes a restaurant from hUBERt
 *@param restaurants
 *Restaurant to remove
 *@param address
 *Address of the restaurant to remove
 */

Restaurant *removeRestaurant(Restaurant* restaurants, Address address);

#endif
