#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "linkedlist.h"
#include "game_setup.h"
#include "tiles.h"
#include "board.h"

int main(){
  while(1){
    setup();
  }
  struct node *start = setup();
  int i, j;
  for(i = 0; i < 15; i++){
    for(j = 0; j < 15; j++){
      change_char_value(start, i, j, 'A');
    }
  }

  print_board(start);
  free_list(start);
  return 0;
}
