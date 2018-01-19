#include "entity.h"

size_t countDishes(Dish* dishes) {
  size_t s;
  for(s = 0; !IS_END_OF_DISHES(dishes); s++, dishes++);
  return ++s;
}
