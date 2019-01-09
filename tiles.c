#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tiles.h"
#include "linkedlist.h"

/*
struct node* tiles_setup(int player_num){
  struct node* letter_list = generate_all_tiles();

}*/

struct node* generate_all_tiles(){
  int i,j;
                            //a b c d e  f g h i j k l m n o p q r s t u v w x y z ('a' should be 9, but is already initialized below)
  int letter_frequency[26] = {8,2,2,4,12,2,3,2,9,1,1,4,2,6,8,2,1,6,4,6,4,2,2,1,2,1};
  struct node *letter_list = insert(NULL, 'A', 0, 0), *temp = letter_list;
  for(i = 0; i < 26; i++){
    for(j = 0; j < letter_frequency[i]; j++) temp = insert(temp, 'A' + i, 0, 0);
  }
  return letter_list;
}

char get_random_tile(struct node** letter_list){
  srand(time(NULL));
  int index = rand() % (get_length(*letter_list));
  char new_c = get_char_value(*letter_list, index);
  *letter_list = remove_node(*letter_list, index);
  return new_c;
}

struct node* generate_initial_player_tiles(struct node** letter_list){
  int i;
  struct node* player_tiles = NULL;
  for(i = 0; i < 7; i++){
    char new_c = get_random_tile(letter_list);
    player_tiles = insert(player_tiles, new_c, 0, 0);
  }
  return get_node(player_tiles, -6, 0);
}

int get_letter_value(char c){
  int letter_values[26] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};
                         //a b c d e f g h i j k l m n o p q  r s t u v w x y z

  return letter_values[c - 'A'];
}

void print_letter_values(){
    printf("\tPoint values for each letter:\n");
    printf("\ta | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q  | r | s | t | u | v | w | x | y | z |\n");
    printf("\t-------------------------------------------------------------------------------------------------------|\n");
    printf("\t1 | 3 | 3 | 2 | 1 | 4 | 2 | 4 | 1 | 8 | 5 | 1 | 3 | 1 | 1 | 3 | 10 | 1 | 1 | 1 | 1 | 4 | 4 | 8 | 4 | 10|\n\n");
}
