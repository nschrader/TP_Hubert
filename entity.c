#include "entity.h"

size_t countDishes(Dish* dishes) {
  size_t s;
  for(s = 0; *((int*) dishes) != 0; s++, dishes++);
  return ++s;
}
