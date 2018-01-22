/**
 * @file
 * A singly-linked list to save restaurant addresses
 */

#ifndef RESTAURANT_LIST_H
#define RESTAURANT_LIST_H

#include "message_queue.h"

/**
 * @struct Restaurant
 * Represents a restaurants
 * @var Restaurant::address
 * Address of the restaurant
 * @var Restaurant::next
 * pointer to next restaurant
 */
typedef struct _Restaurant {
  Address address;
  struct _Restaurant *next;
} Restaurant;

/**
 * Adds a restaurant to list
 * @param restaurants
 * List to add to
 * @param address
 * Address of the restaurant to add
 * @returns
 * The new list to use
 */
Restaurant* addRestaurant(Restaurant* restaurants, Address address);

/**
 * Removes a restaurant from list
 * @param restaurants
 * List to remove from
 * @param address
 * Address of the restaurant to remove
 * @returns
 * The new list to use
 */
Restaurant *removeRestaurant(Restaurant* restaurants, Address address);

#endif
