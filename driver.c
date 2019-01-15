#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "linkedlist.h"
#include "game_setup.h"
#include "tiles.h"
#include "board.h"
#include "player.h"

int main(){
  struct node *start;
  struct Player** all_players = setup(&start, 4);



  /*struct node *random = insert(NULL, 'z', 0, 0), *temp = random;
  random = insert(random, 'u', 0, 0);
  random = insert(random, 'l', 0, 0);
  random = insert(random, 'u', 0, 0);
  random = insert(random, 's', 0, 0);
  //random = insert(random, 'a', 0, 0);
  //random = insert(random, 'r', 0, 0);
  //random = insert(random, 'k', 0, 0);

  //int i;
  //for(i = 1; i < 15; i++){
  //  random = insert(random, i + 'a', 0, 0);
  //}

  if(check_word_validity(temp, 0)) printf("word is real!\n");
  else printf("word is not real!\n");
  */



  /*
  code for gameplay:
  user inputs:
    starting node, direction, and word to add
    computer finds all the differences
      starts at starting node
      if it is '\0', then add it to list of words to check in search_word
      if it is not '\0', make sure user's input matches current board
        if not, return error
      search all added letters in player tiles
        if not found, return error
    find all possible words, check validity of each one while calculating score



  */

  free_list(start);
  return 0;
}
