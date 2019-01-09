#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"
#include "game_setup.h"
#include "tiles.h"
#include "board.h"

struct node* setup(){
  struct node* start = board_setup();
  struct node* player_one = tiles_setup();
  return start;
}
