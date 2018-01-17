#ifndef ENTITY_H
#define ENTITY_H

struct _Menu;
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
} Restaurant;

typedef struct _Dish {
  char name[10];
  int price; //cents
} Dish;

typedef struct _Order {
  struct _User* client;
  struct _Dish* dishes;
  int dishesN;
} Order;

typedef struct _User {
  int id;
  char* name;
} User;

#endif
