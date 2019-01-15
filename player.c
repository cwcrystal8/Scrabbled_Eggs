#include <stdio.h>
#include <stdlib.h>

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
  colors[0] = "\x1b[107m";

  int i;
  for(i = 0; i < 3; i++){
    struct node* temp = player->tiles;
    char filler = ' ', letter = get_char_value(temp, 0);
    if(i != 1) printf("                  ");
    else printf("Player %d's Tiles: ", player_num);
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
    if(i != 2) printf("\x1b[30m%s   %c   \x1b[0m\n", colors[special], filler);
    else if(value > 0 && value < 10) printf("\x1b[30m%s%d  %c   \x1b[0m\n", colors[special], value, filler);
    else if(value == 10) printf("\x1b[30m%s%d %c   \x1b[0m\n", colors[special], value, filler);
    else printf("\x1b[30m%s   %c   \x1b[0m\n", colors[special], filler);
  }
  printf("\n");
}

void set_score(struct Player* player, int num){
  player->score = num;
}

int get_score(struct Player* player){
  return player->score;
}
