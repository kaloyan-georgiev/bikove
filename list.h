#ifndef LIST_HH
#define LIST_HH

struct node_t {
	int value;
	struct node_t* next;
};

struct list_t {
	struct node_t* head;
};

void push_front(struct list_t* list, int value);
void push_back(struct list_t* list, int value);
int index(const struct list_t * const list, int index);
void destroy_list(struct list_t* list);
int detach(struct list_t* list, struct node_t* node);

#endif


