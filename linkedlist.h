struct node;

void print_list(struct node *current);

struct node * insert(struct node * start, char new_value, int position, int special_value);

struct node * free_list(struct node *);

void print_board(struct node *current);

void change_char_value(struct node *start, int i, int j, char new_value);

void change_special_value(struct node *start, int i, int j, char new_value);

struct node* get_node(struct node *start, int i, int j);

struct node * remove_node(struct node *start, int index);

char get_char_value(struct node *start, int index);

int get_length(struct node* start);

void print_board_list(struct node* current, int place);

int search_word(struct node* start, char* letters);
