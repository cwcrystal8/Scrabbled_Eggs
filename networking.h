#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

#ifndef NETWORKING_H
#define NETWORKING_H

#define BUFFER_SIZE 256
#define PORT "9001"
#define TEST_IP "127.0.0.1"

struct package{
  char board[225]; //Or however many tiles total making up the board
  int z[225]; // Corresponding special values
  int has_placed_tiles;
};

void error_check(int i, char *s);
int server_setup();
int server_connect(int sd);
int client_setup(char * server);

void process(char *s);
void subserver(int from_client);
void compress(struct package* backpack, struct node* letter_list, struct node* board);
void extract(struct package* backpack, struct node* letter_list, struct node* board);
struct package* make_package(struct node* letter_list, struct node* board);

#endif
