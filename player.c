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
  //print_list(new_player->tiles);
  return new_player;
}

struct node * get_tiles(struct Player* player){
  return player->tiles;
}

void print_tiles(struct Player* player){
  print_list(player->tiles);
}

void set_score(struct Player* player, int num){
  player->score = num;
}

int get_score(struct Player* player){
  return player->score;
}
