#include "list.h"


typedef struct node {
	struct node *next;
	void *data;
} node_t;


typedef struct list {
	node_t *top;
	node_t *end;
	int sz;
} list_t;


static node_t* create_node(void *data_ptr){
	node_t *node = malloc(sizeof(node_t));
	node->data = data_ptr;
	node->next = NULL;
	return node;
}


list_t* create_list(){
	list_t *list = malloc(sizeof(list_t));
	list->top = NULL;
	list->end = NULL;
	list->sz = 0;
	return list;
}


void list_push_back(list_t *list, void *data_ptr){
	++list->sz;
	if(list->end == NULL){
		list->end = create_node(data_ptr);
		list->top = list->end;
		return;
	}
	list->end->next = create_node(data_ptr);
	list->end = list->end->next;
}


#if 0
void list_push_front(list_t *list, void *data_ptr){
	node_t *tmp = list->top;
	list->top = create_node(data_ptr);
	list->top->next = tmp;
}
#endif


void* list_pop_front(list_t *list){
	node_t *tmp = list->top;
	void* data_ptr = tmp->data;
	list->top = list->top->next;
	if(list_empty(list)) list->end = NULL;
	free(tmp);
	return data_ptr;
}


int list_empty(const list_t* list){
	return list->top == NULL;
}


void list_clean(list_t *list){
	while(!list_empty(list))
		list_pop_front(list);
}
