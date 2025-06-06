#define BASIC_VECTOR_TYPES
#include "../vector.h"
#include <string.h>
#include <time.h>

void print_vector(struct int_vector vec){
	// Now I know this is very unusual in C, but this is an example
	// of how you can use a "lambda function" in the macro parse_vector
	// It uses macros, check the code in "../vector.h" if you're perplexed
	putchar('[');
	parse_vector(vec,
		({
			printf("%d%s",v_element,(v_i == v_size-1)?"]\n":", ");
		})
	);
}

int main(void){
	// Starting value of the vector
	int values[] = {0,1,1,2,3,5,8,13,21,34,55,89};
	struct int_vector vec = (struct int_vector) create_vector();

	// Transfer those values
	for(int i = 0; i < sizeof(values)/sizeof(int); i++){
		push_back(vec,values[i]);
	}

	printf("Fibonacci:\n");
	print_vector(vec);

	// Now, lets try to find the number 5
	size_t result;
	find_vector(vec,(v_element == 5),result);
	// We parse through every element of the vector, until we find the one element where the condition becomes true.
	// Then, the third argument (which should a variable of any integer type) is set to the index
	// If the element is not in the vector, the third argument is set to ~0 which is basically the same as -1

	// But wait, how can a size_t (unsigned long int) be negative? Actually, negative numbers can also be interpreted as unsigned in C.
	// You can still set a size_t number and compare it with -1 because its the same value as ~0, which can be unsigned
	if(result != -1){
		// If the result is not -1, its the index of the element in the vector
		// Now, let's remove the number 5 from the list to see what happens...
		pop_at(vec,result);
	}

	printf("Fibonacci (without 5):\n");
	print_vector(vec);

	// Ta-Da! Now 5 is no more!
	// Let's now engage in some serious stuff...
	// Let's clear the vector and now add random numbers to the array, then sort them ascendingly

	// Clear the vector
	free_vector(vec); // NOTE: Freeing does the same as clearing the vector

	// Lets add a random number of random numbers to the vector
	srand(time(NULL)); // Initialize the random number generator

	// A number between 1 and 20;
	unsigned int n = rand() % 20 + 1;
	// Count down until we reach 0
	for(;n > 0; n--){
		push_back(vec,rand()%2048); // A number between 0 and 2047
	}

	printf("Random numbers:\n");
	print_vector(vec);

	// Let's now sort those numbers, shall we?
	// Another condition we have to input manually again
	// If v_a > v_b, then we switch them
	// The macro does that until the vector is sorted
	// Very bad optimisation, but its made to work with macros, so it needs to be simple
	sort_vector(vec,(v_a > v_b));

	printf("Random numbers (sorted):\n");
	print_vector(vec);

	free_vector(vec);
	return 0;
}