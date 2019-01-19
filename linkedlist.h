struct node;

void print_list(struct node *current);

struct node * insert(struct node * start, char new_value, int position, int special_value);

struct node * free_list(struct node *);

void print_board(struct node *current);

void change_char_value(struct node *start, int i, int j, char new_value);

void change_special_value(struct node *start, int i, int j, char new_value);

int get_special_value(struct node *start, int i, int j);

struct node* get_node(struct node *start, int i, int j);

struct node * remove_node(struct node *start, int index);

char get_vertical_char_value(struct node *start, int index);

char get_char_value(struct node *start, int index);

int get_length(struct node* start);

int get_vertical_length(struct node* start);

void print_board_list(struct node* current, int place);

int search_word(struct node* start, char* letters);

int check_word_validity(struct node* start, int direction);

int check_all_words_validity(struct node* start, int direction);

int get_horizontal_word(struct node* start);

int get_vertical_word(struct node* start);

int check_all_words_validity(struct node* start, int direction);

int get_word_sum(struct node* start, int direction);

int sum_horizontal_word(struct node* start);

int sum_vertical_word(struct node* start);

int add_new_word_points(struct node* start, int direction);
