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

//\e[1;1H\e[2J is the clear screen function
//\x1b[106m is light cyan
//\x1b[102m is light green
//\x1b[0m is return to normal

// int free_all = 0;
// static void sighandler(int signo){
//   if (signo == SIGINT){
//     free_all = 1; // Will free all allocated memory
//     exit(0);
//   }
// }

int main(){
  int center_is_empty = 1;
  struct node *start;
  struct node* letter_list = generate_all_tiles();
  printf("\e[1;1H\e[2J\n\n");
  char num[3];
  num[0] = '0';
  printf("\t\t~~~~~~~~~~~~~~~~~~~~~~~~Welcome to Scrabbled Eggs!~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
  while (num[0] - '0' < 1 ||  num[0] - '0' > 4){
    printf("\t\tHow many players are there?\n\n\t\tThe # of players is ");
    fgets(num, 3, stdin);
    if(num[0] - '0' < 1 ||  num[0] - '0' > 4){
      printf("\n\t\tYou must enter a number between 1 and 4, inclusive!\n\n\n\t\t-----------\n\n\n");
    }
  }
  struct Player** all_players = setup(&start, num[0] - '0', &letter_list);

  while(letter_list){
    printf("\x1b[0m\e[1;1H\e[2J\n\n");
    /*
    printf("\t\tWhose turn is it? Please enter an option from below without the parentheses.\n");
    int l;
    for(l = 1; l <= num[0] - '0'; l++) printf("\t\t(%d) Player %d\n", l, l);
    printf("\t\t(%d) Exit the game\n\n", l);
    char buf[15];

    printf("\t\tI chose ");
    fgets(buf, 15, stdin);

    if(buf[0] == num[0] + 1) break;

    int player_num = buf[0] - '0';*/

    char buf[15];
    int player_num = 1;
    for(player_num = 1; player_num && letter_list; player_num = (player_num) % (num[0] - '0') + 1){
      printf("\e[1;1H\e[2J\n\n");
      printf("\n\t\t-------------PLAYER %d's TURN HAS BEGUN-------------\n\n", player_num);
      print_board(start);
      struct Player *player = all_players[player_num - 1];
      print_tiles(player, player_num);

      buf[0] = 0;
      int is_turn = 1;
      while(is_turn){
        //printf("this is buf[0]: [%c] and in int form: %d\n\n", buf[0], buf[0]);
        printf("\t\tWhat would you like to do? Enter an option from below:\n");
        printf("\t\t(1) Add a word to the board\n");
        printf("\t\t(2) See the manual\n");
        printf("\t\t(3) Pass my turn\n");
        printf("\t\t(4) See the board\n");
        printf("\t\t(5) See my tiles\n");
        printf("\t\t(6) Exit the game\n");
        printf("\t\t(7) Display player scores\n\n");
        //printf("\t\t()\n");
        printf("\t\tI want to chose option ");
        fgets(buf, 15, stdin);
        printf("\n");
        if(buf[0] == '1'){
          printf("\e[1;1H\e[2J\n");
          print_board(start);
          printf("\n\n");
          print_tiles(player, player_num);
          printf("\n\t\tWould you like to add tiles (1) vertically or (2) horizontally?\n\n");
          printf("\t\tI chose option ");
          char buf2[15];
          fgets(buf2, 15, stdin);

          if(buf2[0] == '1'){
            char square[3], word[15];
            int i = 16, j = 16;
            print_board(start);
            if(center_is_empty){
              printf("\t\tYou must start at the center square (7H)!\n");
              i =  7;
              j = 7;
            }
            else{
              printf("\n\t\tWhich square marks the beginning of your word? Please answer number first then the letter\n");
              printf("\t\te.g. 8G, 7H, 14A\n\n");
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
            }
            //printf("\n\t\tYour square has been determined as row %d, column %d\n", j, i);
            printf("\n");
            print_tiles(player, player_num);
            printf("\n\t\tWhat word would you like to form? Please enter it in ALL CAPS\n\n");
            printf("\t\tMy word is ");
            fgets(word, 15, stdin);

            char letters[15], old_word[15];
            int counter = 0, ary_counter = 0, is_valid = 1;
            for(counter; counter < 15 && word[counter] != '\n' && is_valid; counter++){
              if(j+counter >= 15)is_valid = 0;
              else{
                char current_char = get_vertical_char_value(get_node(start, i, j+counter), 0);
                char desired_char = word[counter];
                old_word[counter] = current_char;
                if(current_char == '\0'){
                  //printf("in here!\n");
                  //char a = *letters_copy, b = *comparison;
                  //a = b;
                  letters[ary_counter] = word[counter];

                  ary_counter++;
                  //printf("finished in here!\n");
                  letters[ary_counter] = '\0';
                  //printf("%s\n", letters);
                }
                else if(current_char != desired_char){
                  is_valid = 0;
                }
              }
            }
            if(is_valid){
              word[counter] = '\0';
              if(search_word(get_tiles(player), letters)){
                //printf("those words are valid! removing tiles now... \n");
                //remove tiles from player

                //ADD LETTERS TO BOARD

                //printf("tiles removed!\n\n new tiles\n");
                //print_tiles(player, player_num);
                //struct node* starting_node = get_node(start, i, j);
                int k;
                //printf("HERE");
                int additional_score = 0;
                int original_score = 0;
                int plain_score = 0;
                int multi = 1;
                //printf("added this word: [%s], length is %ld\n", word, strlen(word) -1 );
                for(k = 0; k < strlen(word); k++){
                  //printf("changing tile at row %d, col %d to %c", j+k, i, word[k]);
                  //VERTICAL
                  //printf("before");
                  change_char_value(start, i, j+k, word[k]);
                  if (word[k] != old_word[k]){
                    //printf("WORD %d \n ", word[k]);
                    additional_score += get_letter_value(word[k]);
                  }
                  else{
                    additional_score += add_new_word_points(start,i,j+k,1);
                  }
                  //printf("additional_score %d \n ", additional_score);
                  
                  // getting original score of placed tiles including bonuses
                  if (get_special_value(get_node(start,i,j+k),0,0) == 0){
                    plain_score += get_letter_value(word[k]);
                    original_score +=  get_letter_value(word[k]);
                  }
                  if (get_special_value(get_node(start,i,j+k),0,0) == 1){
                    plain_score += get_letter_value(word[k]);
                    original_score +=  2 * get_letter_value(word[k]);
                  }
                  else if(get_special_value(get_node(start,i,j+k),0,0) == 2) {
                    plain_score += get_letter_value(word[k]);
                    original_score += 3* get_letter_value(word[k]);
                  }
                  else if (get_special_value(get_node(start,i,j+k),0,0) == 3){
                    plain_score += get_letter_value(word[k]);
                    original_score += get_letter_value(word[k]);
                    multi = 2;
                  }
                  else if (get_special_value(get_node(start,i,j+k),0,0) == 4){
                    printf("%d \n ",3);
                    plain_score += get_letter_value(word[k]);
                    original_score += get_letter_value(word[k]);
                    multi = 3;
                  }
                  
                  //printf("im here 1");
                  
                }

                int total_score = additional_score - plain_score + (original_score * multi);
                if(check_all_words_validity(get_node(start, i, j), 1)){
                  //printf("\e[1;1H\e[2J\n");
                  printf("\n\t\tError: You must enter a valid word!\n");
                  for(k = 0; k < strlen(word) ; k++){
                    //printf("changing tile at row %d, col %d to %c", j+k, i, word[k]);
                    change_char_value(start, i, j+k, old_word[k]);
                  }
                }
                else{
                  if(remove_tiles(player, letters, &letter_list)) printf("ERROR REMOVING TILES");
                  else{
                    for(k = 0; k < strlen(word); k++){
                      //printf("changing tile at row %d, col %d to %c", j+k, i, word[k]);
                      change_special_value(start, i, j + k, 6);
                    }
                    is_turn = 0;
                    set_score(player,get_score(player) + total_score);
                    print_board(start);
                    center_is_empty = 0;
                  }

                }
                //printf("finished changing the board!\n");

              }
              else{
                printf("\n\t\tYou do not have the tiles for this addition!\n\n");
              }
            }
            else{
              printf("\n\t\tError: This word does not fit!\n\n");
            }
          }
          else if(buf2[0] == '2'){
            char square[3], word[15];
            int i = 16, j = 16;
            print_board(start);
            if(center_is_empty){
              printf("\t\tYou must start at the center square (7H)!\n");
              i = 7;
              j = 7;
            }
            else{
              printf("\n\t\tWhich square marks the beginning of your word? Please answer number first then the letter\n");
              printf("\t\te.g. 8G, 7H, 14A\n\n");
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
            }
            //printf("\n\t\tYour square has been determined as row %d, column %d\n", j, i);
            printf("\n");
            print_tiles(player, player_num);
            printf("\n\t\tWhat word would you like to form? Please enter it in ALL CAPS\n\n");
            printf("\t\tMy word is ");
            fgets(word, 15, stdin);

            char letters[15], old_word[15];
            int counter = 0, ary_counter = 0, is_valid = 1;
            for(counter; counter < 15 && word[counter] != '\n' && is_valid; counter++){
              if(i+counter >= 15) is_valid = 0;
              else{
                char current_char = get_vertical_char_value(get_node(start, i+counter, j), 0);
                char desired_char = word[counter];
                old_word[counter] = current_char;
                if(current_char == '\0'){
                  //printf("in here!\n");
                  //char a = *letters_copy, b = *comparison;
                  //a = b;
                  letters[ary_counter] = word[counter];
                  ary_counter++;
                  //printf("finished in here!\n");
                  letters[ary_counter] = '\0';
                  //printf("%s\n", letters);

                }
                else if(current_char != desired_char){
                  is_valid = 0;

                }
              }
            }
            if(is_valid){
              word[counter] = '\0';
              if(search_word(get_tiles(player), letters)){

                //printf("those words are valid! removing tiles now... \n");
                //remove tiles from player

                //printf("tiles removed!\n\n new tiles\n");
                //print_tiles(player, player_num);
                //struct node* starting_node = get_node(start, i, j);
                int k;
                int additional_score = 0;
                int original_score = 0;
                int plain_score = 0;
                int multi = 1;
                //printf("added this word: [%s], length is %ld\n", word, strlen(word) -1 );
                for(k = 0; k < strlen(word); k++){
                  //printf("changing tile at row %d, col %d to %c", j+k, i, word[k]);
                  change_char_value(start, i+k, j, word[k]);
                  if (word[k] != old_word[k]){
                    additional_score += get_letter_value(word[k]);
                  }

                  else{
                    additional_score += add_new_word_points(start,i+k,j,0);
                  }
                  
                  if (get_special_value(get_node(start,i+k,j),0,0) == 0){
                    plain_score += get_letter_value(word[k]);
                    original_score +=  get_letter_value(word[k]);
                  }
                  if (get_special_value(get_node(start,i+k,j),0,0) == 1){
                    plain_score += get_letter_value(word[k]);
                    original_score +=  2 * get_letter_value(word[k]);
                  }
                  else if(get_special_value(get_node(start,i+k,j),0,0) == 2) {
                    plain_score += get_letter_value(word[k]);
                    original_score += 3* get_letter_value(word[k]);
                  }
                  else if (get_special_value(get_node(start,i+k,j),0,0) == 3){
                    plain_score += get_letter_value(word[k]);
                    original_score += get_letter_value(word[k]);
                    multi = 2;
                  }
                  else if (get_special_value(get_node(start,i+k,j),0,0) == 4){
                    printf("%d \n ",3);
                    plain_score += get_letter_value(word[k]);
                    original_score += get_letter_value(word[k]);
                    multi = 3;
                  }
                  
                }            
                int total_score = additional_score - plain_score + (original_score * multi);
                if(check_all_words_validity(get_node(start, i, j), 0)){
                  //printf("\e[1;1H\e[2J\n");
                  printf("\n\t\tError: You must enter a valid word!\n\n");
                  for(k = 0; k < strlen(word); k++){
                    //printf("changing tile at row %d, col %d to %c", j+k, i, old_word[k]);
                    change_char_value(start, i+k, j, old_word[k]);
                  }
                }
                else{
                  if(remove_tiles(player, letters, &letter_list)) printf("ERROR REMOVING TILES");
                  //ADD LETTERS TO BOARD
                  else{
                    for(k = 0; k < strlen(word); k++){
                      //printf("changing tile at row %d, col %d to %c", j+k, i, word[k]);
                      change_special_value(start, i+k, j, 6);
                    }
                    is_turn = 0;
                    set_score(player,get_score(player) + total_score);
                    center_is_empty = 0;
                    print_board(start);
                  }
                }
                //printf("finished changing the board!\n");

              }
              else{
                printf("\n\t\tYou do not have the tiles for this addition!\n\n");
              }
            }
            else{
              printf("\n\t\tError: This word does not fit!\n\n");
            }
          }
          buf[0] = '1';
          //printf("set to one!!!!!!!!!!!!!!!!\n");
        }
        else if(buf[0] == '2') print_info();
        else if(buf[0] == '3') is_turn = 0;
        else if(buf[0] == '4') print_board(start);
        else if(buf[0] == '5') print_tiles(player, player_num);
        else if(buf[0] == '6'){
          free_list(letter_list);
          free_list(start);

          struct Player* temp = all_players[0];
          int counter = 0;
          while(temp != NULL){
            free(temp);
            counter+=1;
            temp = all_players[counter];
          }
          exit(0);
        }
        else if (buf[0] == '7') {
          for(player_num = 1; player_num < (num[0] - '0') + 1 ; player_num++){
            printf("\t\t Player %d has %d points\n", player_num, get_score(all_players[player_num-1]));
          }
          printf("\n");
        }
      }
      printf("\n\n\t\t-------------PLAYER %d'S TURN HAS ENDED-------------\n\n\n", player_num);
      
    }
  }
  return 0;
}