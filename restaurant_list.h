#ifndef RESTAURANT_LIST_H
#define RESTAURANT_LIST_H

#include "message_queue.h"

typedef struct _Restaurant {
  Address address;
  struct _Restaurant *next;
} Restaurant;

Restaurant* addRestaurant(Restaurant* restaurants, Address address);
Restaurant *removeRestaurant(Restaurant* restaurants, Address address);

#endif
