#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

struct list;
typedef struct list list_t;

list_t* create_list();
void list_push_back(list_t *list, void* data);
void* list_pop_front(list_t *list);
int list_empty(const list_t *list);
void list_clean(list_t *list);




#endif
