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

#include "networking.h"

//\e[1;1H\e[2J is the clear screen function
//\x1b[106m is light cyan
//\x1b[102m is light green
//\x1b[0m is return to normal


struct package* make_package(struct node* letter_list, struct node* board){
  int length = get_length(letter_list);
  struct package* backpack = malloc(sizeof(struct package));
  int i, j;
  for (i = 0; i < 15; i++){
    for(j = 0; j < 15; j++){
      char c = get_vertical_char_value(get_node(board, i, 0), j);
      int special_value = get_special_value(board, i, j);
      backpack->board[j*15 + i] = c;
      backpack->z[j*15 + i] = special_value;
    }
  }
  backpack->has_placed_tiles = 0;
  return backpack;
}
void compress(struct package* backpack, struct node* letter_list, struct node* board){
  int i, j;
  for (i = 0; i < 15; i++){
    for(j = 0; j < 15; j++){
      char c = get_vertical_char_value(get_node(board, i, 0), j);
      int special_value = get_special_value(board, i, j);
      backpack->board[j*15 + i] = c;
      backpack->z[j*15 + i] = special_value;
    }
  }
}

void extract(struct package* backpack, struct node* letter_list, struct node* board){
  for (int i = 0; i < 225; i++){
    int col = i % 15;
    int row = (int) i /15;
    change_char_value(board, col, row, backpack->board[i]);
    change_special_value(board, col, row, backpack->z[i]);
  }
}

// ------------------------------------------------------------------------------------------------------------
void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    process(buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop
  close(client_socket);
  exit(0);
}

void process(char * s) {
  while (*s) {
    if (*s >= 'a' && *s <= 'z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    else  if (*s >= 'A' && *s <= 'Z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    s++;
  }
}

int main(){
  char role[10];
  printf("Please indicate your status: (host/client)\n");
  fgets(role,10,stdin);
  
  //Client Setup:
  if (role[0] == 'c'){

    //Network stuff:
    int server_socket;
    char buffer[BUFFER_SIZE];

   
    server_socket = client_setup( TEST_IP );


    // ----------------------------------------------------------------------------------------------------
    int round_num = 1;
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

    struct package* backpack = malloc(sizeof(struct package));
    backpack->has_placed_tiles = 0;

    // ----------------------------------------------------------------------------------------------------
    
    while(1){
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
      // Read
      read(server_socket, backpack, sizeof(*backpack));
      extract(backpack,letter_list,start);
      printf("received: [%s]\n", buffer);

      char buf[15];
      int player_num = 2;
      num[0] = '2';
      // for(player_num = 1; player_num; player_num = (player_num) % (num[0] - '0') + 1){
        printf("\e[1;1H\e[2J\n\n");
        printf("\n\t\t-------------PLAYER %d's TURN HAS BEGUN-------------\n\n", player_num);
        print_board(start);
        struct Player *player = all_players[player_num - 1];
        print_tiles(player, player_num);

        buf[0] = 0;
        while(buf[0] != '1'){
          //printf("this is buf[0]: [%c] and in int form: %d\n\n", buf[0], buf[0]);
          printf("\t\tWhat would you like to do? Enter an option from below:\n");
          printf("\t\t(1) Add a word to the board\n");
          printf("\t\t(2) See the manual\n");
          printf("\t\t(3) Pass my turn\n");
          printf("\t\t(4) See the board\n");
          printf("\t\t(5) See my tiles\n");
          printf("\t\t(6) Exit the game\n\n");
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
              if(!backpack->has_placed_tiles){
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

              char letters[15];
              int counter = 0, ary_counter = 0;
              for(counter; counter < 15 && word[counter] != '\n'; counter++){
                char current_char = get_vertical_char_value(get_node(start, i, j+counter), 0);
                char desired_char = word[counter];
                if(current_char == '\0'){
                  //printf("in here!\n");
                  //char a = *letters_copy, b = *comparison;
                  //a = b;
                  letters[ary_counter] = word[counter];
                  ary_counter++;
                  //printf("finished in here!\n");
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
                      /*
                      if(check_all_words_validity(get_node(start, i, j), 1)){
                        for(k = 0; k < strlen(word) - 1; k++){
                          //printf("changing tile at row %d, col %d to %c", j+k, i, word[k]);
                          change_char_value(start, i, j+k, word[k]);
                        }
                      }*/
                      backpack->has_placed_tiles = 1;

                      print_board(start);
                      //printf("finished changing the board!\n");
                    }
                  }
                  else{
                    printf("\t\tYou do not have the tiles for this addition!\n\n");
                    buf[0] = '2';
                  }
                }
                else if(current_char != desired_char){
                  printf("\t\tError: This word does not fit!\n");
                }
              }
            }
            else if(buf2[0] == '2'){
              char square[3], word[15];
              int i = 16, j = 16;
              print_board(start);
              if(!backpack->has_placed_tiles){
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

              char letters[15];
              int counter = 0, ary_counter = 0;
              for(counter; counter < 15 && word[counter] != '\n'; counter++){
                char current_char = get_vertical_char_value(get_node(start, i+counter, j), 0);
                char desired_char = word[counter];
                if(current_char == '\0'){
                  //printf("in here!\n");
                  //char a = *letters_copy, b = *comparison;
                  //a = b;
                  letters[ary_counter] = word[counter];
                  ary_counter++;
                  //printf("finished in here!\n");
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
                      backpack->has_placed_tiles = 1;
                      //printf("finished changing the board!\n");
                    }
                  }
                  else{
                    printf("\t\tYou do not have the tiles for this addition!\n\n");
                    buf[0] = '2';
                  }
                }
                else if(current_char != desired_char){
                  printf("Error: This word does not fit!\n");
                }
              }
            }
            buf[0] = '1';
            //printf("set to one!!!!!!!!!!!!!!!!\n");
          }
          else if(buf[0] == '2') print_info();
          else if(buf[0] == '3') buf[0] = '1';
          else if(buf[0] == '4') print_board(start);
          else if(buf[0] == '5') print_tiles(player, player_num);
          else if(buf[0] == '6'){
            free_list(start);
            exit(0);
          }
        }
        printf("\n\n\t\t-------------PLAYER %d'S TURN HAS ENDED-------------\n\n", player_num);
      // CHANGED ROUND_SUM TO INSIDE THE LOOP
      round_num++;
      compress(backpack,letter_list,start);
      write(server_socket, backpack, sizeof(*backpack));
    }
    
  }
  else{
    int listen_socket;
    int f;
    listen_socket = server_setup();

    int client_socket = server_connect(listen_socket);
    // ------------------------------------------------------------------------------------------------
    // CHANGE has_placed_tiles to PACKAGE.HAS_PLACED_TILES
    int round_num = 1;
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
    
    int package_exists = 0;
    struct package* backpack;
    while(1){
      printf("\x1b[0m\e[1;1H\e[2J\n\n");

      if (!package_exists){
        backpack = make_package(letter_list,start);
        package_exists = 1;
      }
      else{
        read(client_socket, backpack, sizeof(*backpack));
        extract(backpack,letter_list,start);
      }
      printf("[server %d] received\n", getpid());
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
      num[0] = '1';
      int player_num = 1;
      
        printf("\e[1;1H\e[2J\n\n");
        printf("\n\t\t-------------PLAYER %d's TURN HAS BEGUN-------------\n\n", player_num);
        print_board(start);
        struct Player *player = all_players[player_num - 1];
        print_tiles(player, player_num);

        buf[0] = 0;
        while(buf[0] != '1'){
          //printf("this is buf[0]: [%c] and in int form: %d\n\n", buf[0], buf[0]);
          printf("\t\tWhat would you like to do? Enter an option from below:\n");
          printf("\t\t(1) Add a word to the board\n");
          printf("\t\t(2) See the manual\n");
          printf("\t\t(3) Pass my turn\n");
          printf("\t\t(4) See the board\n");
          printf("\t\t(5) See my tiles\n");
          printf("\t\t(6) Exit the game\n\n");
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
              if(!backpack->has_placed_tiles){
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

              char letters[15];
              int counter = 0, ary_counter = 0;
              for(counter; counter < 15 && word[counter] != '\n'; counter++){
                char current_char = get_vertical_char_value(get_node(start, i, j+counter), 0);
                char desired_char = word[counter];
                if(current_char == '\0'){
                  //printf("in here!\n");
                  //char a = *letters_copy, b = *comparison;
                  //a = b;
                  letters[ary_counter] = word[counter];
                  ary_counter++;
                  //printf("finished in here!\n");
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
                      /*
                      if(check_all_words_validity(get_node(start, i, j), 1)){
                        for(k = 0; k < strlen(word) - 1; k++){
                          //printf("changing tile at row %d, col %d to %c", j+k, i, word[k]);
                          change_char_value(start, i, j+k, word[k]);
                        }
                      }*/


                      print_board(start);
                      //printf("finished changing the board!\n");
                    }
                  }
                  else{
                    printf("\t\tYou do not have the tiles for this addition!\n\n");
                    buf[0] = '2';
                  }
                }
                else if(current_char != desired_char){
                  printf("\t\tError: This word does not fit!\n");
                }
              }
            }
            else if(buf2[0] == '2'){
              char square[3], word[15];
              int i = 16, j = 16;
              print_board(start);
              if(player_num == 1 && round_num == 1){
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

              char letters[15];
              int counter = 0, ary_counter = 0;
              for(counter; counter < 15 && word[counter] != '\n'; counter++){
                char current_char = get_vertical_char_value(get_node(start, i+counter, j), 0);
                char desired_char = word[counter];
                if(current_char == '\0'){
                  //printf("in here!\n");
                  //char a = *letters_copy, b = *comparison;
                  //a = b;
                  letters[ary_counter] = word[counter];
                  ary_counter++;
                  //printf("finished in here!\n");
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
                      backpack->has_placed_tiles = 0;
                      //printf("finished changing the board!\n");
                    }
                  }
                  else{
                    printf("\t\tYou do not have the tiles for this addition!\n\n");
                    buf[0] = '2';
                  }
                }
                else if(current_char != desired_char){
                  printf("Error: This word does not fit!\n");
                }
              }
            }
            buf[0] = '1';
            //printf("set to one!!!!!!!!!!!!!!!!\n");
          }
          else if(buf[0] == '2') print_info();
          else if(buf[0] == '3') buf[0] = '1';
          else if(buf[0] == '4') print_board(start);
          else if(buf[0] == '5') print_tiles(player, player_num);
          else if(buf[0] == '6'){
            free_list(start);
            exit(0);
          }
        }
        printf("\n\n\t\t-------------PLAYER %d'S TURN HAS ENDED-------------\n\n", player_num);
      
      round_num++;
      compress(backpack,letter_list,start);
      write(client_socket, backpack, sizeof(*backpack));
    }
  }
  return 0;
}
