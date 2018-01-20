#ifndef RESTAURANT_H
#define RESTAURANT_H

typedef struct {
  int restaurant_id;
  FILE *menu;
} Restaurant;

Restaurant* initRestaurant(int id,FILE *f);
void readMenu(FILE *file);


#endif
