#include "linkedlist.h"

struct Player;

struct Player* create_player(struct node** letter_list);

struct node * get_tiles(struct Player* player);

void print_tiles(struct Player* player);

void set_score(struct Player* player, int num);

int get_score(struct Player* player);
