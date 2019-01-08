#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

struct node {
  char c;
  int special; //0 is regular, 1 is double letter, 2 is triple letter, 3 is double word, 4 is triple word
  struct node *right;
  struct node *left;
  struct node *up;
  struct node *down;
};

struct node* get_node(struct node *start, int i, int j){
  struct node *temp = start;
  for(i; i > 0; i--) temp = temp -> right;
  for(j; j > 0; j--) temp = temp -> down;
  for(i; i < 0; i++) temp = temp -> left;
  for(j; j < 0; j++) temp = temp -> up;
  return temp;
}

void change_char_value(struct node *start, int i, int j, char new_value){
  struct node *temp = start;
  for(i; i > 0; i--) temp = temp -> right;
  for(j; j > 0; j--) temp = temp -> down;
  for(i; i < 0; i++) temp = temp -> left;
  for(j; j < 0; j++) temp = temp -> up;
  temp->c = new_value;
}

void change_special_value(struct node *start, int i, int j, char new_value){
  struct node *temp = start;
  for(i; i > 0; i--) temp = temp -> right;
  for(j; j > 0; j--) temp = temp -> down;
  for(i; i < 0; i++) temp = temp -> left;
  for(j; j < 0; j++) temp = temp -> up;
  temp->special = new_value;
}

void print_list(struct node *current){
  char *colors[6];
  colors[0] = "\x1b[107m";
  colors[1] = "\x1b[104m";
  colors[2] = "\x1b[44m";
  colors[3] = "\x1b[45m";
  colors[4] = "\x1b[41m";
  colors[5] = "\x1b[102m";
  printf("[");
  while((current -> right) != NULL){
    printf("\x1b[30m%s%c\x1b[0m ", colors[current->special], current -> c);
    current = current -> right;
  }
  printf("\x1b[30m%s%c\x1b[0m]", colors[current->special], current -> c);

  printf("\n");
  /*
  printf("[");
  while((current -> right) != NULL){
    if(current->special) printf("\x1b[32m\x1b[31m%c\x1b[0m, ", current -> c);
    else printf("%c, ", current -> c);
    current = current -> right;
  }
  if(current->special) printf("\x1b[32m\x1b[31m%c\x1b[0m]", current -> c);
  else printf("%c]", current -> c);
  printf("\n");*/
}

void print_board(struct node *current){
  while((current -> down) != NULL){
    print_list(current);
    current = current -> down;
  }
  print_list(current);
}

struct node * insert(struct node * start, char new_value, int position, int special_value){
  struct node* new = malloc(4*sizeof(struct node *) + sizeof(char));

  new->down = NULL;
  new->up = NULL;
  new->left = NULL;
  new->right = NULL;
  new -> c = new_value;
  new -> special = special_value;
  if(!start) return new;
  else if(position == 0){
      start->right = new;
      new->left = start;

      if((start->up) != NULL){
        new->up = (start->up)->right;
        ((start->up)->right) -> down = new;
      }
  }
  else if(position == 1){
    start->down = new;
    new->up = start;
  }

  return new;
}

struct node * free_list(struct node *current){
  while(current){
    struct node* start = current;
    while(start){
      struct node* temp = (start -> right);
      free(start);
      start = temp;
    }
    current = current -> down;
  }
  return current;
}
