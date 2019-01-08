#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"
#include "game_setup.h"


struct node* create_board(){
  struct node *start = insert(NULL, '\0', 0, 0), *temp_row = start;
  int i, j;
  for(i = 0; i < 15; i++){
    struct node *temp_col = temp_row;
    for(j = 0; j < 14; j++){
      temp_col = insert(temp_col,'\0', 0, 0);
    }
    if(i < 14) temp_row = insert(temp_row, '\0', 1, 0);
  }
  return start;
}

struct node* set_special_squares(struct node *start){
  //double letter
  int double_letter_x[4] = {7,1,4,5}, double_letter_y[4] = {4,1,0,1};
  set_special_squares_specific(start, double_letter_x, double_letter_y, 1);

  return start;
}

void set_special_squares_specific(struct node *start, int *x, int *y, int special_value){
  int i;
  for(i = 0; i < 4; i++){
    int first = x[i], second = y[i];
    struct node* center = get_node(start, 7, 7);
    change_special_value(center, first, second, special_value);
    change_special_value(center, first, -1* second, special_value);
    change_special_value(center, -1 * first, second, special_value);
    change_special_value(center, -1 * first, -1 * second, special_value);
    first = y[i];
    second = x[i];
    change_special_value(center, first, second, special_value);
    change_special_value(center, first, -1* second, special_value);
    change_special_value(center, -1 * first, second, special_value);
    change_special_value(center, -1 * first, -1 * second, special_value);
  }
}
