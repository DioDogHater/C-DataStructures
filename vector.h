#ifndef CDS_VECTOR_H
#define CDS_VECTOR_H

#include <stdio.h>
#include <stdlib.h>

// Vector structure macros
// A vector structure must have these 2 members:
// <type>* arr, <any int type> size
// Where <type> is the type of the elements stored in the vector
// And <any int type> should be a valid integer type
// A vector should always be initialized with arr=NULL and size=0

// Initialize a vector
#define create_vector() {NULL,0}

// Clear / free a vector
#define free_vector(v) ({ if((v).size) free((v).arr); (v).arr = NULL; (v).size = 0; })

// Push an element e at the back of vector v
#define push_back(v, e) ({\
	(v).arr = realloc((v).arr,sizeof(typeof(e))*++(v).size);\
	(v).arr[(v).size-1] = (e);\
})

// Add a new element to the vector, but with a specified type size
// Does not insert the newly added element in the array
#define add_size_vector(v, s) ({\
	(v).arr = realloc((v).arr,(s)*++(v).size);\
})


// Get nth element of vector
#define at(v, n) ((v).arr[(n)])

// Get nth element of vector, using specified type size
// Meant for vectors whose .arr member is char* or unsigned char*
#define at_sized(v, n, s) ((v).arr+(s)*(n))

// Get last element of vector
#define at_back(v) ((v).arr[(v).size-1])

// Pop last element off the vector
#define pop_back(v) ({\
	if((v).size > 0)(v).arr = realloc((v).arr,sizeof(typeof(*(v).arr))*--(v).size);\
})

// Pop last element off the vector with sized elements
#define pop_back_sized(v,s) ({\
	if((v).size > 0)(v).arr = realloc((v).arr,(s)*--(v).size);\
})

// Pop nth element off the vector
// Shifts all next elements by <- 1
#define pop_at(v,n) ({\
	for(int v_i = (n); v_i < (v).size-1; v_i++){\
		(v).arr[v_i] = (v).arr[v_i+1];\
	}\
	if((v).size > 0)(v).arr = realloc((v).arr,sizeof(typeof(*(v).arr))*--(v).size);\
})

// Pop nth element off the vector, with sized elements
// Shifts all next elements by <- 1
#define pop_at_sized(v,n,s) ({\
	for(int v_i = (n); v_i < (v).size-1; v_i++){\
		*at_sized((v),v_i,(s)) = *at_sized((v),v_i+1,(s));\
	}\
	if((v).size > 0)(v).arr = realloc((v).arr,(s)*--(v).size);\
})

/*
Parse through vector, checking for condition c with element v_element
If condition is true for an element, variable passed as r will be set to current index
Else, will be set to -1 or ~0 (they're the same thing in the eyes of the computer)
Example of usage:

int result;
struct int_vector v = (struct int_vector) create_vector();
find_vector(v,v_element == 5,result);
printf("five is at index: %d\n",result);
*/
#define find_vector(v, c, r) ({\
	typeof(*(v).arr) v_element;\
	r = ~0;\
	for(typeof((v).size) v_i = 0; v_i < (v).size; v_i++){\
		v_element = (v).arr[v_i];\
		if((c)){\
			r = v_i;\
			break;\
		}\
	}\
})

/*
Sort the vector using condition c with elements v_a and v_b
If condition is true, switch out two elements
Else, just skip it
Parse through the vector until everything is at the right place
Very unefficient, worst case scenario O(n^2)
Example of usage:

struct int_vector v = (struct int_vector) create_vector();
sort_vector(v,v_a > v_b); // Sorts the array ascendingly

-- Do something with the sorted vector --
*/
#define sort_vector(v, c) ({\
	typeof(*(v).arr) v_a, v_b;\
	_Bool v_sorted=0;\
	while(!v_sorted){\
		v_sorted=1;\
		for(typeof((v).size) v_i = 0; v_i < (v).size-1; v_i++){\
			v_a = (v).arr[v_i];\
			v_b = (v).arr[v_i+1];\
			if((c)){\
				(v).arr[v_i+1] = v_a;\
				(v).arr[v_i] = v_b;\
				v_sorted = 0;\
			}\
		}\
	}\
})

/*
Parse through vector, executing statements c
You can refer to current element of vector with v_element
You can refer to the current index with v_i
You can refer to the vector's array with v_arr
You can refer to the vector's size with v_size
You can use variables inside the scope where you used the macro
Example of usage:

struct int_vector v = (struct int_vector) create_vector();
int sum = 0;
parse_vector(v,
	({
		printf("number %d: %d\n",v_i,v_element);
		printf("Next element: %d\n", v_i<v_size ? v_arr[v_i+1] : -1);
		sum += v_element;
	})
);
printf("%d\n",sum);
*/
#define parse_vector(v, c) ({\
	typeof(*(v).arr) v_element;\
	typeof((v).arr) v_arr = (v).arr;\
	typeof((v).size) v_size = (v).size;\
	for(typeof((v).size) v_i = 0; v_i < (v).size; v_i++){\
		v_element = v_arr[v_i];\
		(c);\
	}\
})

// Basic vector types
#ifdef BASIC_VECTOR_TYPES
struct int_vector{
	int* arr;
	size_t size;
};

struct float_vector{
	float* arr;
	size_t size;
};

struct char_vector{
	char* arr;
	size_t size;
};

struct bool_vector{
	_Bool* arr;
	size_t size;
};
#endif

#endif