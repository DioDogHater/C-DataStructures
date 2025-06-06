#ifndef CDS_BINARY_TREE_H
#define CDS_BINARY_TREE_H

// A binary tree is a data structure used to represent data that branches in two possible directions
// Here is a visual representation:
//
// 	root node
//         / \
//        a   b
//       / \ / \
//      c  d e  f
//         ...
//
// A binary tree node structure should:
// 	- Be defined and have a size before using macros/functions
// 	- Have at least two members:
// 		* .left : Pointer to the left child node
// 		* .right : Pointer to the right child node
// 		!! These members should always be the two first in the structure
// 	- Normally contain data (otherwise what's the point)
// 	- By definition have a maximum of 2 children
// 	- Pointers need to default to NULL when they are not set
/* EXAMPLE OF BINARY TREE NODE STRUCTURE
struct int_binary_tree {
	void *left, *right;
	int value;
};
OR
binary_tree_with(int value) int_binary_tree;
*/

// Template to create a binary tree node type easily
// Example: typedef binary_tree_with(int number) number_tree_t;
#define binary_tree_with(data) struct { void *left, *right; data; }

// Create a new binary tree node
// You can use extra arguments to set other members of the new node
#define create_binary_tree_node(...) {NULL,NULL,##__VA_ARGS__}

// Macro that is called when a binary tree node is removed or should be freed
// Override with what you want it to be
#ifndef BINARY_TREE_FREE_NODE
#define BINARY_TREE_FREE_NODE(n)
#endif

// Template type to represent a binary tree node with no data involved
struct bt_template_t{
	void* left;
	void* right;
};

// Free children nodes from the parent node, recursively
// Pass root node to free the entire tree
void free_binary_tree(void* parent_ptr){
	if(parent_ptr){
		struct bt_template_t* parent = (struct bt_template_t*) parent_ptr;
		if(parent->left)
			free_binary_tree(parent->left);
		if(parent->right)
			free_binary_tree(parent->right);
		BINARY_TREE_FREE_NODE(parent_ptr);
	}
}

// Access left / right member of tree node
// (n) is a pointer to the node
#define binary_tree_left(n) (typeof((n)))((n)->left)
#define binary_tree_right(n) (typeof((n)))((n)->right)

// A function that will add a new node to a sorted binary tree
// You need to create a function that takes three node pointers
// 1. The node to add to the binary tree
// 2. The potential parent node
// It needs to return a _Bool (aka bool)
// if false, will try to add node as left child, if true, will try add node as right child
// Recursive
/* EXAMPLE
_Bool check_bigger(void* p, void* n){
	return ((node*)(n))->number > ((node*)(p))->number;
}

node* root_node = ...
add_binary_tree_node(root_node,new_node,check_bigger);
*/
void add_binary_tree_node(void* parent_ptr, void* node_ptr, _Bool (*comparison_func)(void*,void*)){
	if(parent_ptr == NULL || node_ptr == NULL) return;
	struct bt_template_t *parent = parent_ptr, *node = node_ptr;
	if(comparison_func(parent_ptr,node_ptr)){
		if(parent->right)
			add_binary_tree_node(parent->right,node,comparison_func);
		else
			parent->right = node;
	}else{
		if(parent->left)
			add_binary_tree_node(parent->left,node,comparison_func);
		else
			parent->left = node;
	}
}

// Cursor movement and clearing the terminal
#define BINARY_TREE_CURSOR(x,y) printf("\033[%d;%dH",(y),(x))
#define BINARY_TREE_CLEAR() printf("\e[1;1H\e[2J")

// Prints out a binary tree at position x and y
// You need to create a function with the pointer of the node to print as an argument
// It will need to return the horizontal delay for the next character
/* EXAMPLE
int print_number_tree_node(void* node){
	printf("%d",((node*)node)->number);
	return 3;
}

node* root_node = ...
print_binary_tree(root_node,0,0,print_number_tree_node);
*/
#define print_binary_tree(n,x,y,pf) ({BINARY_TREE_CLEAR(); print_binary_tree_ex((n),(x),(y),(pf)); putchar('\n');})
int print_binary_tree_ex(void* n, int x, int y, int (*printing_func)(void*)){
	if(!n) return x;
	if(((struct bt_template_t*)n)->left){
		x = print_binary_tree_ex(((struct bt_template_t*)n)->left,x,y+2,printing_func);
		BINARY_TREE_CURSOR(x-1,y+1);
		putchar('/');
	}
	BINARY_TREE_CURSOR(x,y);
	x += printing_func(n);
	if(((struct bt_template_t*)n)->right){
		BINARY_TREE_CURSOR(x,y+1);
		putchar('\\');
		x = print_binary_tree_ex(((struct bt_template_t*)n)->right,x,y+2,printing_func);
	}
	return x;
}

#endif
