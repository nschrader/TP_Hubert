#ifndef ENTITY_H
#define ENTITY_H

#include <stddef.h>

#define NAME_BUFFER 10

typedef struct {
  int id;
  char name[NAME_BUFFER];
  int price; //cents
  int quantity;
} Dish;
#define IS_END_OF_DISHES(x) (*((int*) x) == 0)

typedef int Order;
#define IS_END_OF_ORDERS(x) (*x == 0)

size_t countDishes(Dish* dishes);
size_t countOrders(Order* orders);

#endif
