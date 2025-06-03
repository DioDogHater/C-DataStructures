#include "../vector.h"
#include "../hashtable.h"
#include <string.h>

#define bool _Bool
#define true 1
#define false 0

typedef struct{
	char* key; // Their name
	unsigned int age; // Their age
} person_pair_t;

typedef struct{
	person_pair_t* arr;
	size_t size;
} person_vector_t;

// My hashing function
size_t hashfunc(size_t ht_size, void* element){
	person_pair_t pair = * (person_pair_t*) element;
	size_t index = 0;
	for(int i = 0; i < strlen(pair.key); i++){
		index += (size_t)pair.key[i];
	}
	return index % ht_size;
}

int main(void){
	// Create a hashtable with a maximum of 16 elements per index and 5 starting indices
	hashtable_t ht = create_ht(
		hashfunc,
		16,
		sizeof(person_pair_t)
	);
	setup_ht(&ht,5);

	// Get input from the user
	// Asks them to input people
	printf("Enter quit in the name field to stop entering names!\n");
	while(true){
		// Get the name and age
		char name[50];
		unsigned int age;
		printf("Please enter the new person's name:\n");
		fgets(name,49,stdin);
		name[strlen(name)-1] = '\0';
		// Check if the user wants to finish typing data
		if(!strcmp(name,"quit")) break;
		// Ask them for their age
		printf("Please enter that person's age:\n");
		scanf("%d",&age);
		getchar(); // Skip the endline character left by scanf()
		// Allocate the key (name) of the person and create the pair
		person_pair_t new_person = (person_pair_t){
			malloc(strlen(name)),
			age
		};
		// Copy the temporary buffer's data into the pair's key
		memcpy(new_person.key,name,strlen(name));
		// Add the pair to the hashtable
		add_ht(&ht,&new_person);
		printf("Added person:\n%s -> %u years old.\n",new_person.key,new_person.age);
	}

	printf("Hashtable has %lu hashsets\n",ht.size);
	for(int i = 0; i < ht.size; i++){
		printf("Hashset %d has %lu elements\n",i,at(ht,i).size);
	}

	// Sort people with their age ascendingly
	// Parse through all the pairs in the hashtable and add them to a vector
	person_vector_t vec = (person_vector_t) create_vector();
	parse_ht(ht,({
		person_pair_t pair = * (person_pair_t*) h_element;
		push_back(vec,pair);
	}));
	sort_vector(vec,(v_a.age > v_b.age));

	// Print the list of people, sorted
	printf("List of all registered people, from youngest to oldest:\n");
	parse_vector(vec,({
		printf("- %s - %u years old.\n",v_element.key,v_element.age);
	}));

	// Free the vector
	free_vector(vec);

	// Now let the user try to find people's information in the hashtable
	printf("Enter quit to stop the program!\n");
	while(true){
		// Get the person's name
		char name[50];
		printf("Please enter a person's name to find in the database:\n");
		fgets(name,49,stdin);
		name[strlen(name)-1] = '\0';
		if(!strcmp(name,"quit")) break;

		// Search for that name in the hashtable
		size_t result;
		person_pair_t person = (person_pair_t){name};
		// find_ht(hashtable, what to find, condition, resulting pair, resulting index)
		find_ht(ht,person,!strcmp(h_element.key,h_target.key),person,result);
		if(result != -1){ // If the result is positive, it must mean we found it
			printf("%s is %u years old.\n",person.key,person.age);
		}else // Otherwise, it's not in the hashtable
			printf("%s was not found!\n",name);
	}
	
	// Free all pairs in the hashtable, since their keys are allocated
	parse_ht(ht,({
		person_pair_t pair = * (person_pair_t*) h_element;
		free(pair.key);
	}));
	free_ht(ht);

	return 0;
}