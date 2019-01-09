#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"
#include "tiles.h"

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

char get_char_value(struct node *start, int index){//for tiles only
  struct node* temp = get_node(start, index, 0);
  return temp->c;
}

int get_length(struct node* start){//for tiles only
  int ans = 0;
  struct node* temp = start;
  while(temp){
    ans++;
    temp = temp->right;
  }
  return ans;
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
  printf("[");
  while((current -> right) != NULL){
    printf("%c ", current -> c);
    current = current -> right;
  }
  printf("%c]", current -> c);
  printf("\n");
}

void print_board_list(struct node* current, int place){
  char *colors[6];
  colors[0] = "\x1b[107m";
  colors[1] = "\x1b[104m";
  colors[2] = "\x1b[44m";
  colors[3] = "\x1b[45m";
  colors[4] = "\x1b[41m";
  colors[5] = "\x1b[102m";
  int i;
  for(i = 0; i < 3; i++){
    struct node* temp = current;
    char filler = ' ';
    if(i == 1 && place < 10) printf("   %d   ", place);
    else if(i == 1) printf("  %d   ", place);
    else printf("       ");
    int value = get_letter_value(temp->c);
    while((temp -> right) != NULL){
      value = get_letter_value(temp->c);
      if(i == 1 && temp->c != '\0') filler = temp -> c;
      else filler = ' ';
      if(i != 2) printf("\x1b[30m%s   %c   \x1b[0m  ", colors[temp->special], filler);
      else if(value < 10) printf("\x1b[30m%s%d  %c   \x1b[0m  ", colors[temp->special], value, filler);
      else if(value == 10) printf("\x1b[30m%s%d %c   \x1b[0m  ", colors[temp->special], value, filler);
      else printf("\x1b[30m%s   %c   \x1b[0m  ", colors[temp->special], filler);
      temp = temp -> right;
    }
    if(i != 2) printf("\x1b[30m%s   %c   \x1b[0m\n", colors[temp->special], filler);
    else if(value < 10) printf("\x1b[30m%s%d  %c   \x1b[0m\n", colors[temp->special], value, filler);
    else if(value == 10) printf("\x1b[30m%s%d %c   \x1b[0m\n", colors[temp->special], value, filler);
    else printf("\x1b[30m%s   %c   \x1b[0m\n", colors[temp->special], filler);
  }
  printf("\n");
}

void print_board(struct node *current){
  int i;
  printf("          A        B        C        D        E        F        G        H        I        J        K        L        M        N        O   \n\n");
  for(i = 0; i < 15; i++){
    print_board_list(current, i);
    current = current -> down;
  }
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

struct node * remove_node(struct node *start, int index){ //for tiles only
  struct node* target = get_node(start, index, 0);
  if(index == 0){ //if it is the beginning
    struct node* temp = target->right;
    temp->left = NULL;
    free(target);
    return temp;
  }
  if(!target->right){//if it is the end
    (target->left)->right = NULL;
    free(target);
    return start;
  }
  (target->left)->right = target->right;
  (target->right)->left = target->left;
  free(target);
  return start;
}

/*
int search(struct node* start, char* letters){
  int i, j, is_used[7] = {0,0,0,0,0,0,0};
  for(i = 0; i < sizeof(letters)/sizeof(char); i++){
    for(j = 0; j < 7; j++){
      if(letters[i] == get_char_value(start, j))
    }
  }
}
*/
