#include <stdlib.h>
#include <stdio.h>
#include "rb_tree.h"

int main(void){
	setbuf(stdout, NULL);
	struct rb_tree *tree = create_rb_tree();
	
	for(int i = 0; i<15; i+=1)
		rbt_insert(tree, i);
		
	rbt_print_postorder(tree);
	rbt_print_preorder(tree);
	
	rbt_delete(tree, 7);
	rbt_print_postorder(tree);
	rbt_print_preorder(tree);

	rbt_insert(tree, 15);
	rbt_print_postorder(tree);
	rbt_print_preorder(tree);

} 
	
/* [ids@archlinux cnpct]$ gcc rb_tree.c list.c rb_tree_test.c -o rb_tree_test.e -lm */
/* [ids@archlinux cnpct]$ ./rb_tree_test.e */
/* __postorder:__ */
/* 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 */

/* __preorder:__ */
/*                                                                3 */
/*                                            1                                           7 */
/*                          0                         2                         5                         9 */
/*               n              n              n              n              4              6              8             11 */
/*        n       n       n       n       n       n       n       n       n       n       n       n       n       n      10      13 */
/*    n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n  12  14 */
/*  n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n */

/* __postorder:__ */
/* 0 1 2 3 4 5 6 8 9 10 11 12 13 14 */

/* __preorder:__ */
/*                                                                3 */
/*                                            1                                           8 */
/*                          0                         2                         5                         9 */
/*               n              n              n              n              4              6              n             11 */
/*        n       n       n       n       n       n       n       n       n       n       n       n       n       n      10      13 */
/*    n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n   n  12  14 */
/*  n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n */

/* [ids@archlinux cnpct]$ */

