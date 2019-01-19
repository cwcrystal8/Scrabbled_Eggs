#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "player.h"
#include "linkedlist.h"
#include "tiles.h"

struct Player {
  int score;
  struct node *tiles;
};


struct Player* create_player(struct node** letter_list){
  struct Player* new_player = calloc(1, sizeof(int) + sizeof(struct node *));
  new_player->score = 0;
  new_player->tiles = generate_initial_player_tiles(letter_list);
  return new_player;
}

struct node * get_tiles(struct Player* player){
  return player->tiles;
}

void print_tiles(struct Player* player, int player_num){
  char *colors[6];
  colors[0] = "\x1b[103m";

  int i;
  for(i = 0; i < 3; i++){
    struct node* temp = player->tiles;
    char filler = ' ', letter = get_char_value(temp, 0);
    if(i != 1) printf("\t\t                  ");
    else printf("\t\tPlayer %d's Tiles: ", player_num);
    int value = get_letter_value(letter), special;
    while(temp){
      letter = get_char_value(temp, 0);
      value = get_letter_value(letter);
      special = get_special_value(temp, 0, 0);
      value = get_letter_value(letter);
      if(i == 1 && letter != '\0') filler = letter;
      else filler = ' ';
      if(i != 2) printf("\x1b[30m%s   %c   \x1b[0m  ", colors[special], filler);
      else if(value > 0 && value < 10) printf("\x1b[30m%s%d  %c   \x1b[0m  ", colors[special], value, filler);
      else if(value == 10) printf("\x1b[30m%s%d %c   \x1b[0m  ", colors[special], value, filler);
      else printf("\x1b[30m%s   %c   \x1b[0m  ", colors[special], filler);
      temp = get_node(temp, 1, 0);
    }
    /*
    if(i != 2) printf("\x1b[30m%s   %c   \x1b[0m\n", colors[special], filler);
    else if(value > 0 && value < 10) printf("\x1b[30m%s%d  %c   \x1b[0m\n", colors[special], value, filler);
    else if(value == 10) printf("\x1b[30m%s%d %c   \x1b[0m\n", colors[special], value, filler);
    else printf("\x1b[30m%s   %c   \x1b[0m\n", colors[special], filler);*/
    printf("\n");
  }
  printf("\n");
}

void set_score(struct Player* player, int num){
  player->score = num;
}

int get_score(struct Player* player){
  return player->score;
}

int remove_tiles(struct Player *player, char *letters, struct node **letter_list){
  struct node* player_tiles = get_tiles(player);
  int i, j, is_looking = 1, length = get_length(player_tiles);
  int is_old_tiles[length];

  for(i = 0; i < length; i++) is_old_tiles[i] = 1;

  //printf("LOOKING FOR WORD %s\n",letters);
  for(i = 0 ; i < strlen(letters); i++){
    char target = letters[i];
    is_looking = 1;
    for(j = 0; j < length; j++){
      struct node *current_tile = get_node(player_tiles, j, 0);
      char current_char = get_char_value(current_tile, 0);
      if(current_char == target && is_looking && is_old_tiles[j]){
        //printf("looking for %c, found %c, is_looking: %d\n", target, current_char, is_looking);
        change_char_value(current_tile, 0, 0, get_random_tile(letter_list));
        is_looking = 0;
        is_old_tiles[j] = 0;
      }
    }
    if(is_looking) return 1;
  }
  return 0;
}
