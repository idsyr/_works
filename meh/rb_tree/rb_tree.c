#include "rb_tree.h"

typedef enum color {BLACK = 0, RED} color_t; 

typedef struct node {
	struct node *parent;
	struct node *left;
	struct node *right;
	color_t color;
	int val;
} node_t;


typedef struct rb_tree {
	node_t *nil;
	node_t *top;
	int sz;
} rb_tree_t;


static node_t* create_node( int val){
	node_t *node = malloc(sizeof(node_t));
	node->parent = NULL;
	node->left   = NULL;
	node->right  = NULL;
	node->color  = RED;
	node->val    = val;
	return node;
}


rb_tree_t *create_rb_tree(){
	rb_tree_t *rb_tree =  malloc(sizeof(rb_tree_t));
	rb_tree->nil = create_node(42);
	rb_tree->nil->color = BLACK;
	rb_tree->top = NULL;
	rb_tree->sz = 0;
	return rb_tree;
}


static void take_parent(rb_tree_t *tree, node_t *src_node, node_t *rcvd_node){
	if(src_node->parent == tree->nil) {
		tree->top = rcvd_node;
		tree->nil->left = rcvd_node;
		tree->nil->right = rcvd_node;
		rcvd_node->parent = tree->nil;}
	else if(src_node == src_node->parent->left)
		src_node->parent->left  = rcvd_node;
	else
		src_node->parent->right = rcvd_node;
	if(rcvd_node != NULL) rcvd_node->parent = src_node->parent;
}


static void set_right(node_t *src_node, node_t *rcvd_node){
	rcvd_node->right = src_node;
	if(src_node != NULL)
		rcvd_node->right->parent = rcvd_node;
}


static void set_left(node_t *src_node, node_t *rcvd_node){
	rcvd_node->left = src_node;
	if(src_node != NULL)
		rcvd_node->left->parent = rcvd_node;
}


static void left_rotate(rb_tree_t *tree, node_t *x){
	node_t *xr = x->right;
	if(x->parent == tree->nil) {tree->top = xr;}
	take_parent(tree, x, xr);
	set_right(xr->left, x);
	set_left(x, xr);
}


static void right_rotate(rb_tree_t *tree, node_t *x){
	node_t *xr = x->left;
	if(x->parent == tree->nil) tree->top = xr;
	take_parent(tree, x, xr);
	set_left(xr->right, x);
	set_right(x, xr);
}


static void rbt_insert_fixup(rb_tree_t *tree, node_t *path_node){
	while(path_node->parent != NULL &&
	      path_node->parent->parent != NULL &&
	      path_node->parent->color == RED){

		int left_fixup = path_node->parent == path_node->parent->parent->left;
		node_t *uncle = (left_fixup)?
			path_node->parent->parent->right : path_node->parent->parent->left;
		color_t uncle_color = (uncle == NULL)? BLACK : uncle->color;
		
		if(uncle_color == RED){
			path_node->parent->color = BLACK;
			uncle->color = BLACK;
			path_node->parent->parent->color = RED;
			path_node = path_node->parent->parent;
		} else {
			if(path_node == ((left_fixup)?
				path_node->parent->right : path_node->parent->left)){
				path_node = path_node->parent;
				(left_fixup)? left_rotate(tree, path_node) : right_rotate(tree, path_node);
			 }
			path_node->parent->color = BLACK;
			path_node->parent->parent->color = RED;
			(left_fixup)?
				right_rotate(tree, path_node->parent->parent) : left_rotate(tree, path_node->parent->parent);
		}
		
	}
}


void rbt_insert(rb_tree_t *tree, int val){
	node_t *new_node = create_node(val);
	node_t *path_node = tree->top;

	++tree->sz;

	if(tree->top == NULL) {
		new_node->color = BLACK;
		tree->top = new_node;
		tree->nil->left = new_node;
		tree->nil->right = new_node;
		new_node->parent = tree->nil;
		return;} 

	for(;;){
		if(path_node->val == val) {
			free(new_node);
			return;}

		if(path_node->val > val){
			if(path_node->left == NULL) {
				set_left(new_node, path_node);
				break;}
			path_node = path_node->left;
		}

		if(path_node->val < val){
			if(path_node->right == NULL) {
				set_right(new_node, path_node);
				break;}
			path_node = path_node->right;
		}
	}
	rbt_insert_fixup(tree, new_node); 
}


static node_t* find_node(rb_tree_t *tree, int val){
	node_t *path_node = tree->top;
	while(path_node != NULL){
		if(path_node->val == val) return path_node;
		if(path_node->val > val) path_node = path_node->left;
		else                     path_node = path_node->right;
	}
	return NULL;
}


static node_t* rb_minimum(node_t *path_node){
	while(path_node->left != NULL)
		path_node = path_node->left;
	return path_node;
}


int rbt_find(rb_tree_t *tree, int val){
	node_t *node = find_node(tree, val);
	return (node == NULL)? -1 : node->val;
}


static void rbt_delete_fixup(rb_tree_t *tree, node_t *path_node){
	while(path_node != NULL && path_node->color == BLACK){

		int left_fixup = path_node == path_node->parent->left;
		node_t *bro_node = (left_fixup)?
			path_node->parent->right : path_node->parent->left;
		void (*straight_rotate)(rb_tree_t*, node_t*) = (left_fixup)?
			left_rotate : right_rotate;
		void (*reverse_rotate)(rb_tree_t*, node_t*) = (left_fixup)?
			right_rotate : left_rotate;
		
		if(bro_node->color == RED){
			bro_node->color = BLACK;
			path_node->parent->color = RED;
			straight_rotate(tree, path_node->parent);
			bro_node = (left_fixup)?
				path_node->parent->right : path_node->parent->left;}
		if(bro_node->left->color == BLACK && bro_node->right->color == BLACK){
			bro_node->color = RED;
			path_node = path_node->parent;}
		else{
			if(((left_fixup)? bro_node->right->color : bro_node->left->color) == BLACK){
				(left_fixup)? (bro_node->left->color = BLACK) : (bro_node->right->color = BLACK);
				bro_node->color = RED;
				reverse_rotate(tree, path_node->parent);
				bro_node = (left_fixup)?
					path_node->parent->right : path_node->parent->left;}
			bro_node->color = path_node->parent->color;
			path_node->parent->color = BLACK;
			straight_rotate(tree, path_node->parent);
			path_node = tree->top;}
	}
	path_node->color = BLACK;
}


void rbt_delete(rb_tree_t *tree, int val){	
	node_t *rm_node = find_node(tree, val);
	node_t *min_node;
	node_t *fixup_node;
	color_t fixup_color = rm_node->color;
	if(rm_node == NULL) return;

	++tree->sz;

	if(rm_node->left == NULL){
		fixup_node = rm_node->right;
		take_parent(tree, rm_node, rm_node->right);}
	else if(rm_node->right == NULL){
		fixup_node = rm_node->left;
		take_parent(tree, rm_node, rm_node->left);}
	else {
		min_node = rb_minimum(rm_node->right);
		fixup_color = min_node->color;
		if(min_node->parent != rm_node){
			take_parent(tree, min_node, min_node->right);
			set_right(rm_node->right, min_node);
		}
		take_parent(tree, rm_node, min_node);
		set_left(rm_node->left, min_node);
		min_node->color = rm_node->color;
	}
	free(rm_node);
	if(fixup_color == BLACK)
		rbt_delete_fixup(tree, min_node->right);
}


static void print_postorder(node_t* path_node){
	if(path_node == NULL) return;
	print_postorder(path_node->left);
	printf("%d ", path_node->val);
	print_postorder(path_node->right);
}


void rbt_print_postorder(rb_tree_t *tree){
	printf("__postorder:__\n");
	node_t *path_node = tree->top;
	print_postorder(path_node);
	printf("\n\n");
}


static int line_control(int *line_num, int *line_count, int *line_null_count){
	if(*line_num <= *line_count){
			if(*line_num == *line_null_count) return 0;
			*line_num *= 2;
			*line_null_count = 0;
			*line_count = 0;
			printf("\n");
		}
	return 1;
}


void rbt_print_preorder(rb_tree_t *tree){
	printf("__preorder:__\n");

	list_t *queue = create_list();
	list_push_back(queue, tree->top);
	
	int line_width = pow(2, (int)(log(tree->sz+1) / log(2)+3)) + 1;
	int line_num = 1;
	int line_count = 0;
	int line_null_count = 0;
	
	while(!list_empty(queue)){
		++line_count;

		int tabln = line_width/(line_num + 1) + line_width%(line_num);
		
		node_t* tmp = list_pop_front(queue);
		if(tmp == NULL) {
			++line_null_count;
			
			printf(ANSI_COLOR_YELLOW "%*c" ANSI_COLOR_RESET, tabln, 'n');
			list_push_back(queue, tmp);
			list_push_back(queue, tmp);
		} else {
			if(tmp->color==BLACK) printf(ANSI_COLOR_GREEN "%*d" ANSI_COLOR_RESET, tabln, tmp->val);
			else                  printf(ANSI_COLOR_RED   "%*d" ANSI_COLOR_RESET, tabln, tmp->val);
			list_push_back(queue, tmp->left);
			list_push_back(queue, tmp->right);
		}
		
		if(!line_control(&line_num, &line_count, &line_null_count)) break;
	}
	list_clean(queue);
	printf("\n\n");
}


void rbt_clean(rb_tree_t *tree){
	list_t *queue = create_list();
	list_push_back(queue, tree->top);
	while(!list_empty(queue)){
		node_t* tmp = list_pop_front(queue);
		if(tmp == NULL) continue;
		list_push_back(queue, tmp->left);
		list_push_back(queue, tmp->right);
		free(tmp);
	}
	free(tree->nil);
}


