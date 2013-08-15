#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "init.h"
#include "register.h"
#include "screen.h"

int main(void) {
  initializePins();
  bool **screen;
  int col, row;
  screen = (bool**) malloc(UNIT_WIDTH * sizeof(bool*));
  for (row = 0; row < UNIT_WIDTH; row++) {
    screen[row] = (bool*) malloc(UNIT_LENGTH * sizeof(bool));
    for (col = 0; col < UNIT_LENGTH; col++) {
      //Test pattern to make sure data serialization is working as expected.
      if (col % 2 == 0) {
      	screen[row][col] = true;
      } else {
      	screen[row][col] = false;
      }
    }
  }
  
  init(screen);
  free(screen);
  return 0;
}