/**
 * @file
 * Basic entites modeling real life
 */

#ifndef ENTITY_H
#define ENTITY_H

#include <stddef.h>

#define NAME_BUFFER 10 //!< A dish name cannot be longer than this

/**
 * @struct Dish
 * Represents a dish or, in case of an array, a menu
 * @var Dish::id
 * @var Dish::name
 * @var Dish::price
 * The price in cents
 * @var Dish::quantity
 * Only used be restaurant and kitchen to manage stock
 */
typedef struct {
  int id;
  char name[NAME_BUFFER];
  int price; //cents
  int quantity;
} Dish;

/**
 * Determine easily if we have reached the "Null-Byte" indicating end of array
 */
#define IS_END_OF_DISHES(x) (*((int*) x) == 0)

/**
 * An order is nothing but a @ref Dish id
 */
typedef int Order;

/**
 * Determine easily if we have reached the "Null-Byte" indicating end of array
 */
#define IS_END_OF_ORDERS(x) (*x == 0)

/**
 * Count the number of dishes in the array
 * @param dishes
 * A "Null-Byte" terminated array
 * @returns
 * The number of elements
 */
size_t countDishes(Dish* dishes);

/**
 * Count the number of orders in the array
 * @param orders
 * A "Null-Byte" terminated array
 * @returns
 * The number of elements
 */
size_t countOrders(Order* orders);

#endif
