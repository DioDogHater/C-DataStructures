#ifndef CDS_HASHTABLE_H
#define CDS_HASHTABLE_H

// IMPORTANT! THIS HEADER DEPENDS ON "vector.h"!

// We need the vector macros
#include "vector.h"
#include <string.h>

// To use the hashtable provided by this header, you will have to create your own key/value pair structure
// Please add this member to the key/value pair struct
// <key type> key, ...
// Where <key type> is of course the type of the keys you use in the hashtable
// You can then add all values and information you wanna store for each key/value pair
/*
EXAMPLE:

struct hashpair{
	char* key; // The name of a person
	unsigned int phone_number; // Their number
	float height; // height in meters
}
*/

// Vector of key/value pairs, is allocated by the hashtable dynamically
// This vector uses a char array, though it does not act like a character string
struct hashset{
	char* arr;
	size_t size;
};

// This is basically a vector of hashets which are also vectors
struct hashtable{
	struct hashset* arr;
	size_t size;
	size_t (*hashing_func)(size_t,void*); // The hashing function
	unsigned int max_size; // The max amount of pairs in a single index (cannot be 0)
	size_t pair_size; // Size of the key/value pairs, in bytes
};

/*
Creates a hashtable with nothing in it
Will set the hashing function to the value of h
Will set the max amount of values in a single index to m
Argument p needs to be the size of the pairs to be allocated in the hashtable
h must be a function with this format:

size_t hashing_func(size_t size_of_hashtable, void* element_to_be_indexed){
	-- your hashing code --
	return hash key;
}
*/
#define create_ht(h,m,p) (struct hashtable){NULL,0,(h),(m),(p)};

// Frees a hashtable
#define free_ht(h) ({\
	for(int h_i = 0; h_i < (h).size; h_i++){\
		free_vector((h).arr[h_i]);\
	}\
	free_vector((h));\
	(h).arr = NULL;\
	(h).size = 0;\
})

// Add extra hashsets to the hashtable
void add_hashsets_ht(struct hashtable* ht, int size){
	struct hashset hs = (struct hashset) create_vector();
	for(int i = 0; i < size; i++){
		push_back(*ht,hs);
	}
}

// Setup a hashtable with specified starting size
void setup_ht(struct hashtable* ht, int start_size){
	free_ht(*ht);
	add_hashsets_ht(ht,start_size);
}

// Add a key/value pair to the hashtable
// 1. We get the hash index from the element we want to add
// 2. Check if the index is valid, if not throw an error
// 3. Add the element to the hashset with the index we just got
// 4. Check if the max size is respected, if not rearrange the hashtable
// When using this function, please pass in a pointer to the key/value pair as the second argument
void add_ht(struct hashtable* ht, void* element_to_add){
	if(ht->arr == NULL || ht->size == 0 || ht->hashing_func == NULL || ht->max_size == 0) return;
	// Step 1
	size_t index = ht->hashing_func(ht->size,element_to_add);
	// Step 2
	if(index >= ht->size){
		printf("ERROR: Index provided by hashing function is out of range!\n");
		return;
	};
	// Step 3
	struct hashset* hs = &at(*ht,index);
	add_size_vector(*hs, ht->pair_size);
	memcpy(
		at_sized(*hs, hs->size-1, ht->pair_size),
		element_to_add,
		ht->pair_size
	);
	// Step 4
	if(hs->size > ht->max_size){
		size_t old_size = ht->size;
		add_hashsets_ht(ht,ht->size); // Double the size of the hashtable
		for(size_t i = 0; i < old_size; i++){
			struct hashset* hs = &at(*ht,i);
			if(hs->size == 0) continue;
			for(size_t j = hs->size-1; j >= 0 && j < ~0; j--){
				char* element = (char*)malloc(ht->pair_size);
				memcpy(element,at_sized(*hs,j,ht->pair_size),ht->pair_size);
				size_t new_index = ht->hashing_func(ht->size,(void*)element);
				if(new_index != i){
					pop_at_sized(*hs,j,ht->pair_size);
					add_size_vector(at(*ht,new_index),ht->pair_size);
					memcpy(
						at_sized(at(*ht,new_index),at(*ht,new_index).size-1,ht->pair_size),
						element,
						ht->pair_size
					);
				}
				free(element);
			}
		}
	}
}

// Get the vector where the pair passed as 2nd argument should be
// 1. Get the hash index from the element we want to find
// 2. Check if index is valid, throw an error if not
// 3. Just return the hashset as a pointer
struct hashset* get_ht(struct hashtable* ht, void* element_to_get){
	if(ht->arr == NULL || ht->size == 0 || ht->hashing_func == NULL || ht->max_size == 0) return NULL;
	// Step 1
	size_t index = ht->hashing_func(ht->size,element_to_get);
	// Step 2
	if(index >= ht->size){
		printf("ERROR: Index provided by hashing function is out of range!\n");
		return NULL;
	}
	// Step 3
	struct hashset* hs = &at(*ht,index);
	return hs;
}

// Find an key/value pair in a hashset
// First arg is the hashset (not as a pointer!)
// Second arg is the condition to check wether we found the pair we wanted
// The condition can use these local variables as reference points:
// h_element (current element to compare), h_target (element we want to find), h_i (current index in the hashset)
// Third arg is the element (key/value pair) to find (not a pointer!)
// Fourth arg is the "return value" or the result, should be a size_t variable
// NOTE: if the result is equals to -1, the element is not in the hashset!
/* Example:

--- Stuff before ---

struct address{
	char* key; // Postal code
	unsigned int addr_num; // Address number
}

--- Create the hashtable, whatever... ---
struct address element_to_get = (struct address){"A1B2C3"};
struct hashset* hs = get_ht(&ht,&element_to_get);
if(hs == NULL) printf("hashset doesnt exist!\n");
else{
	size_t result;
	find_hs(
		*hs,
		element_to_get
		(!strcmp(v_element.key,v_target.key)),
		result
	);
	if(result == -1) printf("Failed to find the element!\n");
	else printf("Found element at index %lu of hashset!\n",result);
}

*/
#define find_hs(h,e,c,r) ({\
	typeof((e)) h_target = (e);\
	typeof((e)) h_element;\
	r = ~0;\
	for(size_t h_i = 0; h_i < (h).size; h_i++){\
		h_element = * (typeof((e))*) at_sized((h),h_i,sizeof((e)));\
		if((c)){\
			r = h_i;\
			break;\
		}\
	}\
})

// Find a key/value pair in the hashtable
// First arg is the hashtable itself (not a pointer!)
// Second arg is the element (key/value pair) we want to find in the hashtable
// Third arg is the condition to check if elements in hashsets are the one we want to find
// Fourth is the "return value", basically a variable with the same type as the element we want to find
// Fifth is the index of the element in the hashset it was found in. (is equals to -1 when its not found)
// PLEASE CHECK THE VALUE OF THE INDEX TO SEE IF IT WAS FOUND!
/* EXAMPLE:

--- Stuff before ---

struct address{
	char* key; // Postal code
	unsigned int addr_num; // Address number
}

--- Create the hashtable, whatever... ---
struct address element_to_get = (struct address){"A1B2C3"};
struct address found_element;
size_t result;
find_ht(ht, element_to_get, !strcmp(h_element.key,h_target.key), found_element, result);
if(result == -1) printf("Address not found!\n");
else printf("found %s : %d\n",found_element.key,found_element.addr_num);

*/
#define find_ht(h,e,c,s,r) ({\
	struct hashset* hs_find = get_ht(&(h),&(e));\
	if(hs_find == NULL) { printf("Failed to get hashset index!\n"); (r) = ~0; }\
	else{\
		find_hs(*hs_find,(e),(c),(r));\
		if((r) != -1) (s) = *(typeof((s))*)at_sized(*hs_find,(r),sizeof((e)));\
	}\
})

// Parse through all the items of the hashtable
// First arg is the hashtable (not a pointer!)
// Second arg is the code to be executed for each key/value pair in the hashtable
// Use these local variables as references:
/*
- h_element -> void*, is a pointer to the start of the element you are parsing
NOTE: to convert h_element to the key/value pair you are using, simply use this template:
	<type of pair> pair = * (<type of pair>*) h_element;
- h_i -> size_t, is the index of the current hashset inside of the hashtable
- h_hs -> struct hashset*, is the pointer to the current hashset
- h_j -> size_t, is the index of the current element inside of the hashset
*/
#define parse_ht(h,c) ({\
	struct hashset* h_hs;\
	void* h_element;\
	for(size_t h_i = 0; h_i < (h).size; h_i++){\
		h_hs = &at((h),h_i);\
		for(size_t h_j = 0; h_j < h_hs->size; h_j++){\
			h_element = at_sized(*h_hs,h_j,(h).pair_size);\
			(c);\
		}\
	}\
})

#endif