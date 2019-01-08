#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "linkedlist.h"
#include "game_setup.h"

int main(){

  struct node *start = create_board();
  int i, j;
  for(i = 0; i < 15; i++){
    for(j = 0; j < 15; j++){
      change_char_value(start, i, j, 'a');
    }
  }
  set_special_squares(start);



  print_board(start);
  free_list(start);
  return 0;
}
