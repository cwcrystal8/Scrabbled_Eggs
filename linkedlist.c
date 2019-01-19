#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"
#include "tiles.h"
#include "dict.h"

struct node {
  char c;
  int special; //0 is regular, 1 is double letter, 2 is triple letter, 3 is double word, 4 is triple word, 5 is starting tile, 6 is a placed tile
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

char get_vertical_char_value(struct node *start, int index){
  struct node* temp = get_node(start, 0, index);
  return temp->c;
}

int get_vertical_length(struct node *start){
  int ans = 0;
  struct node* temp = start;
  while(temp){
    ans++;
    temp = temp->down;
  }
  return ans;
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

int get_special_value(struct node *start, int i, int j){
  struct node *temp = start;
  for(i; i > 0; i--) temp = temp -> right;
  for(j; j > 0; j--) temp = temp -> down;
  for(i; i < 0; i++) temp = temp -> left;
  for(j; j < 0; j++) temp = temp -> up;
  return temp->special;
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
  colors[6] = "\x1b[103m";
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
      else if(value > 0 && value < 10) printf("\x1b[30m%s%d  %c   \x1b[0m  ", colors[temp->special], value, filler);
      else if(value == 10) printf("\x1b[30m%s%d %c   \x1b[0m  ", colors[temp->special], value, filler);
      else printf("\x1b[30m%s   %c   \x1b[0m  ", colors[temp->special], filler);
      temp = temp -> right;
    }
    if(i != 2) printf("\x1b[30m%s   %c   \x1b[0m\n", colors[temp->special], filler);
    else if(value > 0 && value < 10) printf("\x1b[30m%s%d  %c   \x1b[0m\n", colors[temp->special], value, filler);
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
  if (get_length(start) == 1){
    return NULL;
  }
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

int search_word(struct node* start, char* letters){ //start is player tiles, letters is letters that we are looking for
  int i, j, length = get_length(start), is_used[length], is_here = 0;
  if(strlen(letters) > length) return 0;
  for(i = 0; i < length; i++){
    is_used[i] = 0;
  }
  for(i = 0; i < strlen(letters); i++){
    is_here = 0;
    //printf("i: %d\n", i);
    for(j = 0; j < length; j++){
      //printf("\tj: %d, looking for: %c, current char: %c, is_used[j]: %d, is_here:%d\n", j, letters[i], get_char_value(start, j), is_used[j], is_here);
      if(letters[i] == get_char_value(start, j) && !is_used[j] && !is_here) {
        is_used[j] = 1;
        is_here = 1;
      }
    }
    if(!is_here) return 0;
  }
  return 1;
}

int check_word_validity(struct node* start, int direction){
  //direction 0: horizontal, 1: vertical
  int i, length, offset = 'a' - 'A';
  char word[16];
  if(!direction) length = get_length(start);
  else length = get_vertical_length(start);

  for(i = 0; i < length && ( (get_char_value(start, i) && !direction) || (get_vertical_char_value(start, i) && direction) ); i++){
    if(!direction) word[i] = get_char_value(start, i) + offset;
    else word[i] = get_vertical_char_value(start, i) + offset;
    //printf("\t\t%c\n", word[i]);
  }
  word[i] = '\0';
  //printf("\tthe word is %s, the length is %d, i is %d\n", word, length, i);

  //check if word is real, returns 1 if real, 0 if not
  return valid_word(word);
}

int get_horizontal_word(struct node* start){
  int i;
  for(i = 0; get_char_value(start, i - 1); i--);
  struct node *far_left = get_node(start, i, 0);
  if(!check_word_validity(far_left, 0) && get_char_value(far_left, 1)) return 1; //1 means error
  //printf("Horizontal has no error!%d, %d\n", check_word_validity(far_left, 0), get_char_value(far_left, 1));
  return 0;
}

int get_vertical_word(struct node* start){
  int i;
  for(i = 0; get_vertical_char_value(start, i - 1); i--);
  struct node *far_up = get_node(start, 0, i);
  if(!check_word_validity(far_up, 1) && get_vertical_char_value(far_up, 1)) return 1;
  //printf("vertical has no error!%d, %d\n", check_word_validity(far_up, 1), (far_up->down)->c);
  return 0;
}

int check_all_words_validity(struct node* start, int direction){
  if(!(start->c)) return 1;
  //direction: 0 is horizontal, 1 is vertical
  //assume is horizontal for now
  if(get_horizontal_word(start) || get_vertical_word(start)) return 1;

  struct node* temp;
  if(direction){
    for(temp = start->down; temp->c; temp = temp->down){
      if(get_horizontal_word(temp)) return 1;
    }
  }
  else{
    for(temp = start->right; temp->c; temp = temp->right){
      if(get_vertical_word(temp)) return 1;
    }
  }
  return 0;

}
//////////////////////////////////// new code
int get_word_sum(struct node* start, int direction){
  //direction 0: horizontal, 1: vertical
  int i, length;
  int sum = 0;
  if(!direction) length = get_length(start);
  else length = get_vertical_length(start);
  for(i = 0; i < length && ( (get_char_value(start, i) && !direction) || (get_vertical_char_value(start, i) && direction) ); i++){
    if(!direction){
      //printf("%d %d \n",get_char_value(start, i), get_letter_value(get_char_value(start, i)) );
      sum += get_letter_value(get_char_value(start, i));      
    } 
    else{ 
      //printf("%d %d \n",get_vertical_char_value(start, i), get_letter_value(get_char_value(start, i)) );
      sum += get_letter_value(get_vertical_char_value(start, i));
    }
    //printf("\t\t%c\n", word[i]);
  }
  
  //check if word is real, returns 1 if real, 0 if not
  return sum;
}
int sum_horizontal_word(struct node* start){
  int i;
  for (i = 0; get_char_value(start,i-1);i--);
  struct node *fare_left = get_node(start,i,0);
  //return 0;
  return get_word_sum(fare_left,0);
}
int sum_vertical_word(struct node* start){
  int i;
  for(i = 0; get_vertical_char_value(start, i - 1); i--);
  struct node *far_up = get_node(start, 0, i);
  //return 0;
  return get_word_sum(far_up,1);
}
int add_new_word_points(struct node* start, int i, int j, int direction){
  if (direction){
    return sum_horizontal_word(get_node(start, i, j));
    //return 0;
  }
  else{
   //return 0;
    return sum_vertical_word(get_node(start,  i,  j));
  }  
}