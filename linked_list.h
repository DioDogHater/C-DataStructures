#ifndef CDS_LINKED_LIST_H
#define CDS_LINKED_LIST_H

/* 
Here is a vague definition of a linked list type:
- Should be a structure that is already defined before using it
- Has a member(s) that point to next node
	* Singly linked list :	.next = pointer to next node
	* Doubly linked list :	.next = pointer to next node,
	 			.previous = pointer to previous node
- Pointers should default to NULL when no node is next
- Will always be used in macros/functions as pointers
Always be consistent with your data.
*/

// Macros to quickly define a linked list type with data members
// How to use:
// typedef singly_linked_list_with(int number) number_list_t;
#define singly_linked_list_with(data) struct { void *next; data; }
#define doubly_linked_list_with(data) struct { void *next, *previous; data; }

// Create a singly/doubly linked list node
// You can give more member values as arguments
#define create_singly_list_node(...) { NULL,##__VA_ARGS__ }
#define create_doubly_list_node(...) { NULL,NULL,##__VA_ARGS__ }

// Macro to free/manage list nodes that may be left out
// To handle left out nodes, please create the macro LINKED_LIST_FREE_NODE(n), n being the node pointer, before including this header
/* EXAMPLE:
#define LINKED_LIST_FREE_NODE(n) ({ free((n)); })
#include "linked_list.h"
	...
*/
#ifndef LINKED_LIST_FREE_NODE
#define LINKED_LIST_FREE_NODE(n)
#endif

// Free a singly linked list from (a) to the end of it
// You need to give the start of the list, otherwise the linked list will not be freed correctly
#define free_singly_linked_list(a) ({\
	typeof((a)) ll_ptr = (a);\
	while(ll_ptr){\
		(a) = (typeof((a)))(a)->next;\
		LINKED_LIST_FREE_NODE(ll_ptr);\
		ll_ptr = (a);\
	}\
})

// Free the entirety of the linked list where (a) can be any node inside it
#define free_doubly_linked_list(a) ({\
	linked_list_traverse_left((a),-1);\
	free_singly_linked_list((a));\
})

// Get node after (a)
#define linked_list_next(a) ((typeof((a)))(a)->next)

// Get node after (a)
#define linked_list_previous(a) ((typeof((a)))(a)->previous)

// Adds node (b) at the end of singly or doubly linked list (a)
// CAUTION: Will loop infinitely if last node points to first
#define linked_list_add_end(a,b) ({\
	typeof((a)) ll_ptr = (a);\
	while(ll_ptr->next) { ll_ptr = (typeof((a))) ll_ptr->next; }\
	ll_ptr->next = (b);\
})

// Adds node (b) at the start of doubly linked list (a)
// CAUTION: Will loop infinitely if first node points last
#define linked_list_add_start(a,b) ({\
	typeof((a)) ll_ptr = (a);\
	while(ll_ptr->next) { ll_ptr = (typeof((a))) ll_ptr->previous; }\
	ll_ptr->previous = (b);\
})

// Inserts node (b) after node (a)
#define linked_list_insert_next(a,b) ({\
	if((a)->next) (b)->next = (a)->next;\
	(a)->next = (b);\
})

// Inserts node (b) before node (a)
#define linked_list_insert_previous(a,b) ({\
	if((a)->previous) (b)->previous = (a)->previous;\
	(a)->previous = (b);\
})

// Removes (a)->next from the linked list 
#define linked_list_remove_next(a) ({\
	if((a)->next){\
		typeof((a)) ll_ptr = (typeof((a))) (a)->next;\
		(a)->next = ((typeof((a)))(a)->next)->next;\
		LINKED_LIST_FREE_NODE(ll_ptr);\
	}\
})

// Removes (a)->previous from the linked list
#define linked_list_remove_previous(a) ({\
	if((a)->previous){\
		typeof((a)) ll_ptr = (typeof((a))) (a)->previous;\
		(a)->previous = ((typeof((a)))(a)->previous)->previous;\
		LINKED_LIST_FREE_NODE(ll_ptr);\
	}\
})

// Traverse right from (a), (n) times
// Pass -1 as (n) to traverse to the end of the list (might infinitely loop if last node is linked to first)
// (a) WILL BE SET TO RESULT !!
// If we reach the end of the linked list, (a) = the last node we reached
#define linked_list_traverse_right(a, n) ({\
	unsigned long ll_index = (n);\
	while((a)->next && ll_index){\
		(a) = (typeof((a)))(a)->next;\
		ll_index--;\
	}\
})

// Traverse left from (a), (n) times
// Pass -1 as (n) to traverse to the start of the list (might infinitely loop if first node is linked to last)
// (a) WILL BE SET TO RESULT !!
// If we reach the end of the linked list, (a) = the last node we reached
#define linked_list_traverse_left(a, n) ({\
	unsigned long ll_index = (n);\
	while((a)->previous && ll_index){\
		(a) = (typeof((a)))(a)->previous;\
		ll_index--;\
	}\
})

#endif
