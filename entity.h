#ifndef ENTITY_H
#define ENTITY_H

typedef struct {
  User* client;
  Dish[] offer;
  int offerN;
} Menu;

typedef struct {
  int id;
  char* name;
  Dish[] stock;
  int stockN;
} Restaurant;

typedef struct {
  int id;
  char* name;
  int quantity;
} Dish;

typedef struct {
  User* client;
  Dish[] dishes;
  int dishesN;
} Order;

typedef struct {
  int id;
  char* name;
} User;

#endif
