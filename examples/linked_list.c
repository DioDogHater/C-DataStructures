#include <stdio.h>
#include <stdlib.h>

#define LINKED_LIST_FREE_NODE(n) ({ free((n)); })

#include "../linked_list.h"

// Creates the type of a singly linked list to store numbers
// -> struct { void* next; int number; }
typedef singly_linked_list_with(int number) number_list_t;

int main(void){
	// Create the first ever singly linked list node with numbers
	number_list_t first_number_node = (number_list_t) create_singly_list_node(5); // This means first_number_node.number = 5

	for(int i = 4; i >= -5; i--){
		printf("Adding number to list: %d\n",i);

		// Allocate and create a new node
		number_list_t *new_node = (number_list_t*) malloc(sizeof(number_list_t));
		*new_node = (number_list_t) create_singly_list_node(i);

		// Add the new node to the end of the list
		linked_list_add_end(&first_number_node, new_node);
	}

	// Create a pointer to move around the linked list
	number_list_t* number_node = &first_number_node;
	printf("Traversing right 3 times...\n");
	linked_list_traverse_right(number_node,3); // Traverse right for 3 nodes
	
	// Remove the 3rd node after the first one (should be 1)
	// This macro WILL call the macro LINKED_LIST_FREE_NODE, which in this case will free the node's allocated memory
	printf("Removing next node...\n");
	linked_list_remove_next(number_node);
	
	// Traverse right for 2 nodes
	printf("Traversing right 2 times...\n");
	linked_list_traverse_right(number_node,2);

	// Insert a new node
	printf("Adding number to list: 12345\n");
	// Allocate that new node
	number_list_t* new_node = (number_list_t*) malloc(sizeof(number_list_t));
	*new_node = (number_list_t) create_singly_list_node(12345);

	// Insert it after number_node
	linked_list_insert_next(number_node, new_node);

	// Parse through the linked list and print out each value
	number_node = &first_number_node;
	printf("Number Linked List: [ ");
	while(number_node){
		printf("%d ",number_node->number); // Print out the number
		number_node = linked_list_next(number_node);
	}
	printf("]\n");
	
	// Free the linked list
	// (linked_list_next is to skip first_number_node itself since it wasn't allocated on the heap)
	number_node = linked_list_next(&first_number_node);
	free_singly_linked_list(number_node);

	return 0;
}
