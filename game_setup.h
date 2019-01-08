struct node* create_board();

struct node* set_special_squares(struct node *start);

void set_special_squares_specific(struct node *start, int *x, int *y, int special_value, int len);
