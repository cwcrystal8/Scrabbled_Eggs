//struct node** tiles_setup(int player_num);

int get_letter_value(char c);

void print_letter_values();

struct node* generate_initial_player_tiles(struct node** letter_list);

struct node* generate_all_tiles();

char get_random_tile(struct node** letter_list);
