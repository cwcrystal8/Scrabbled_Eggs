#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>

#include "linkedlist.h"
#include "game_setup.h"
#include "tiles.h"
#include "board.h"
#include "player.h"
#include "dict.h"

int main(){
  struct node *start;
  struct node* letter_list = generate_all_tiles();
  struct Player** all_players = setup(&start, 4, &letter_list);

  while(1){
    print_board(start);
    printf("\t\tWhich player is up? Enter 1, 2, 3, or 4 to select the player. Enter 5 to exit.\n");
    char buf[15];
    printf("\t\t");
    fgets(buf, 15, stdin);

    if(buf[0] == '5') break;

    int player_num = buf[0] - '0';
    printf("\n\t\t-------------PLAYER %d's TURN HAS BEGUN-------------\n\n", player_num);
    struct Player *player = all_players[player_num - 1];
    print_tiles(player, player_num);

    buf[0] = 0;
    while(buf[0] != '1'){
      printf("\t\tWhat would you like to do? Enter an option from below:\n");
      printf("\t\t(1) Add a word to the board\n");
      printf("\t\t(2) See the manual\n");
      printf("\t\t(3) Pass my turn\n");
      printf("\t\t(4) See the board\n\n");
      //printf("\t\t()\n");
      printf("\t\tI want to chose option ");
      fgets(buf, 15, stdin);
      if(buf[0] == '1'){
        printf("\n\t\tWould you like to add tiles (1) vertically or (2) horizontally?\n");
        printf("\t\tI want to chose option ");
        fgets(buf, 15, stdin);
        if(buf[0] == '1'){
          char square[3], word[15];
          int i = 16, j = 16;
          printf("\n\t\tWhich square marks the beginning of your word? Please answer number first then the letter\n");
          printf("\t\te.g. 8G, 7H, 14A\n");
          printf("\t\tMy word begins at square ");
          fgets(square, 15, stdin);
          if(square[2] == '\n'){
            i = square[1] - 'A';
            j = square[0] - '0';
          }
          else if(square[3] == '\n'){
            i = square[2] - 'A';
            j = square[1] - '0' + 10;
          }
          //printf("\n\t\tYour square has been determined as row %d, column %d\n", j, i);
          printf("\n\t\tWhat word would you like to form?\n");
          printf("\t\tMy word is ");
          fgets(word, 15, stdin);

          char letters[15];
          int counter = 0, ary_counter = 0;
          for(counter; counter < 15 && word[counter] != '\n'; counter++){
            char current_char = get_vertical_char_value(get_node(start, i, j+counter), 0);
            char desired_char = word[counter];
            if(current_char != desired_char){
              //printf("in here!\n");
              //char a = *letters_copy, b = *comparison;
              //a = b;
              letters[ary_counter] = word[counter];
              ary_counter++;
              //printf("finished in here!\n");
            }
          }
          letters[ary_counter] = '\0';
          //printf("%s\n", letters);
          if(search_word(get_tiles(player), letters)){
            //printf("those words are valid! removing tiles now... \n");
            //remove tiles from player
            if(remove_tiles(player, letters, &letter_list)) printf("ERROR REMOVING TILES");
            //ADD LETTERS TO BOARD
            else{
              //printf("tiles removed!\n\n new tiles\n");
              print_tiles(player, player_num);
              //struct node* starting_node = get_node(start, i, j);
              int k;
              //printf("added this word: [%s], length is %ld\n", word, strlen(word) -1 );
              for(k = 0; k < strlen(word) - 1; k++){
                //printf("changing tile at row %d, col %d to %c", j+k, i, word[k]);
                change_char_value(start, i, j+k, word[k]);
              }
              print_board(start);
              //printf("finished changing the board!\n");
            }
          }
          else{
            printf("\t\tYou do not have the tiles for this addition!\n\n");
            buf[0] = '2';
          }
        }
        else if(buf[0] == '2'){
          char square[3], word[15];
          int i = 16, j = 16;
          printf("\n\t\tWhich square marks the beginning of your word? Please answer number first then the letter\n");
          printf("\t\te.g. 8G, 7H, 14A\n");
          printf("\t\tMy word begins at square ");
          fgets(square, 15, stdin);
          if(square[2] == '\n'){
            i = square[1] - 'A';
            j = square[0] - '0';
          }
          else if(square[3] == '\n'){
            i = square[2] - 'A';
            j = square[1] - '0' + 10;
          }
          //printf("\n\t\tYour square has been determined as row %d, column %d\n", j, i);
          printf("\n\t\tWhat word would you like to form?\n");
          printf("\t\tMy word is ");
          fgets(word, 15, stdin);

          char letters[15];
          int counter = 0, ary_counter = 0;
          for(counter; counter < 15 && word[counter] != '\n'; counter++){
            char current_char = get_vertical_char_value(get_node(start, i+counter, j), 0);
            char desired_char = word[counter];
            if(current_char != desired_char){
              //printf("in here!\n");
              //char a = *letters_copy, b = *comparison;
              //a = b;
              letters[ary_counter] = word[counter];
              ary_counter++;
              //printf("finished in here!\n");
            }
          }
          letters[ary_counter] = '\0';
          //printf("%s\n", letters);
          if(search_word(get_tiles(player), letters)){
            
            //printf("those words are valid! removing tiles now... \n");
            //remove tiles from player
            if(remove_tiles(player, letters, &letter_list)) printf("ERROR REMOVING TILES");
            //ADD LETTERS TO BOARD
            else{
              //printf("tiles removed!\n\n new tiles\n");
              print_tiles(player, player_num);
              //struct node* starting_node = get_node(start, i, j);
              int k;
              //printf("added this word: [%s], length is %ld\n", word, strlen(word) -1 );
              for(k = 0; k < strlen(word) - 1; k++){
                //printf("changing tile at row %d, col %d to %c", j+k, i, word[k]);
                change_char_value(start, i+k, j, word[k]);
              }
              print_board(start);
              //printf("finished changing the board!\n");
            }
          }
          else{
            printf("\t\tYou do not have the tiles for this addition!\n\n");
            buf[0] = '2';
          }
        }
      }
      else if(buf[0] == '2') print_info();
      else if(buf[0] == '3') buf[0] = '1';
      else if(buf[0] == '4') print_board(start);
    }
    printf("\n\n\t\t-------------PLAYER %d'S TURN HAS ENDED-------------\n\n", player_num);
  }




  /*
  change_char_value(start, 0, 0, 'A');
  change_char_value(start, 1, 0, 'A');
  change_char_value(start, 2, 0, 'R');
  change_char_value(start, 3, 0, 'D');
  change_char_value(start, 4, 0, 'V');
  change_char_value(start, 5, 0, 'A');
  change_char_value(start, 6, 0, 'R');
  change_char_value(start, 7, 0, 'K');
  change_char_value(start, 0, 1, 'R');
  change_char_value(start, 0, 2, 'T');
  change_char_value(start, 1, 1, 'B');
  change_char_value(start, 1, 2, 'S');
  change_char_value(start, 1, 3, 'T');
  change_char_value(start, 1, 4, 'R');
  change_char_value(start, 1, 5, 'A');
  change_char_value(start, 1, 6, 'C');
  change_char_value(start, 1, 7, 'T');
  print_board(start);


  if(check_all_words_validity(get_node(start, 0, 0), 0)) printf("\t\terror!\n");
  else printf("\t\tsuccess!\n");
  */

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

  if(check_word_validity(temp, 0)) printf("\t\tword is real!\n");
  else printf("\t\tword is not real!\n");
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
      if all added letters are not on board, say error
      search all added letters in player tiles
        if not found, return error
    find all possible words, check validity of each one while calculating score



  */

  free_list(start);
  return 0;
}
