#ifndef RB_TREE_H
#define RB_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "list.h"
#include "term_colors.h"

struct rb_tree;
typedef struct rb_tree rb_tree_t;

struct rb_tree* create_rb_tree();
void rbt_insert(struct rb_tree *tree, int val);
void rbt_delete(struct rb_tree *tree, int val);
int rbt_find(struct rb_tree *tree, int val);
void rbt_print_postorder(struct rb_tree *tree);
void rbt_print_preorder(rb_tree_t *tree);
void rbt_clean(rb_tree_t *tree);

#endif 
