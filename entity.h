#ifndef ENTITY_H
#define ENTITY_H

#include <stddef.h>

#define NAME_BUFFER 10

/*struct _Menu;
struct _Restaurant;
struct _Dish;
struct _Order;
struct _User;

typedef struct _Menu {
  struct _User* client;
  struct _Dish* offer;
  int offerN;
} Menu;

typedef struct _Restaurant {
  int id;
  char* name;
  struct _Dish* stock;
  int stockN;
} Restaurant;*/

typedef struct {
  int id;
  char name[NAME_BUFFER];
  int price; //cents
} Dish;

#define IS_END_OF_DISHES(x) (*((int*) x) == 0)

/*typedef struct _Order {
  struct _User* client;
  struct _Dish* dishes;
  int dishesN;
} Order;

typedef struct _User {
  int id;
  char* name;
} User;*/

size_t countDishes(Dish* dishes);

#endif
