#include <stdlib.h>

#include "restaurant_list.h"

static Restaurant* newRestaurant(Address address) {
  Restaurant* new = malloc(sizeof(Restaurant));
  new->address = address;
  new->next = NULL;
  return new;
}

Restaurant* addRestaurant(Restaurant* restaurants, Address address) {
  if (restaurants == NULL) {
    return newRestaurant(address);
  } else {
    Restaurant* list = restaurants;
    while (list->next != NULL) {
      list = list->next;
    }
    list->next = newRestaurant(address);
    return restaurants;
  }
}

Restaurant *removeRestaurant(Restaurant* restaurants, Address address) {
  if (restaurants == NULL) {
    return NULL;
  }
  if (restaurants->address == address) {
    Restaurant* nextRestaurant = restaurants->next;
    free(restaurants);
    return nextRestaurant;
  }
  restaurants->next = removeRestaurant(restaurants->next, address);
  return restaurants;
}
