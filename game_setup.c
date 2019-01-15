#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"
#include "game_setup.h"
#include "tiles.h"
#include "board.h"
#include "player.h"

struct Player** setup(struct node** start, int num_players){
  *start = board_setup();

  /* //TESTING PURPOSES ONLY -- REMOVE LATER

  int i, j;
  for(i = 0; i < 15; i++){
    for(j = 0; j < 15; j++){
      change_char_value(*start, i, j, 'A');
    }
  }
  //END OF TESTING CODE*/
  print_board(*start);

  struct Player** all_players = calloc(num_players, sizeof(struct Player*));
  struct node* letter_list = generate_all_tiles();
  while(num_players > 0){
      all_players[num_players - 1] = create_player(&letter_list);
      num_players--;
  }
  //print_info();
  return all_players;
}

void print_info(){
  printf("\t------GAME INFO------\n\n");
  print_board_info();
  print_letter_values();
}

void print_board_info(){
  printf("\n\tSpecial Squares Key\n\n");
  char *colors[6], *key[6];
  colors[0] = "\x1b[107m";
  colors[1] = "\x1b[104m";
  colors[2] = "\x1b[44m";
  colors[3] = "\x1b[45m";
  colors[4] = "\x1b[41m";
  colors[5] = "\x1b[102m";
  key[0] = "Regular tiles";
  key[1] = "Double-letter tiles";
  key[2] = "Triple-letter tiles";
  key[3] = "Double-word tiles";
  key[4] = "Triple-word tiles";
  key[5] = "Start tile";
  int j, i;
  for(j = 0; j < 6; j++){
    for(i = 0; i < 3; i++){
        printf("\t");
        if (i == 1) printf("\x1b[30m%s      \x1b[0m = %s\n", colors[j], key[j]);
        else printf("\x1b[30m%s      \x1b[0m  \n", colors[j]);
    }
    printf("\n");
  }
}
