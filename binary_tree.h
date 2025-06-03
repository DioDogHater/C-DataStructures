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
binary_tree_with(int value);
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

#endif
