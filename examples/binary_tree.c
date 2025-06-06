#include <stdio.h>
#include <stdlib.h>

#include "../binary_tree.h"

// The type for binary tree numbers
typedef binary_tree_with(int number) number_tree;

// Compare two numbers, to sort binary tree
static _Bool compare_number_tree(void* parent, void* node){
	return ((number_tree*)node)->number > ((number_tree*)parent)->number;
}

// Print out a number_tree node
static int print_number_tree(void* node){
	return printf("%d",((number_tree*)node)->number);
}

// Find number_tree node in tree
number_tree* find_number_tree(number_tree* parent, int number){
	if(parent == NULL) return NULL;
	if(parent->number == number)
		return parent;
	else if(number > parent->number)
		return find_number_tree(binary_tree_right(parent),number);
	else
		return find_number_tree(binary_tree_left(parent),number);
}

int main(void){
	number_tree root_node = (number_tree) create_binary_tree_node(50);

	number_tree nodes[] = {
		create_binary_tree_node(10),
		create_binary_tree_node(25),
		create_binary_tree_node(55),
		create_binary_tree_node(68),
		create_binary_tree_node(-1),
		create_binary_tree_node(100),
		create_binary_tree_node(61),
		create_binary_tree_node(-12),
		create_binary_tree_node(0)
	};

	for(int i = 0; i < sizeof(nodes)/sizeof(number_tree); i++){
		add_binary_tree_node(&root_node,&nodes[i],compare_number_tree);
	}

	print_binary_tree(&root_node,1,1,print_number_tree);

	while(1){
		int number;
		scanf("%d",&number);
		getchar();
		printf("Is %d in the tree? %s\n",number,find_number_tree(&root_node,number)?"true":"false");
	}

	free_binary_tree(&root_node);
}