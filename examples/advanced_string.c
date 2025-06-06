#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../advanced_string.h"

int main(void){
	// Set the maxmimum amount of bytes to allocate
	// everytime we append text to the string
	_STRING_APPEND = 256;

	// Create an empty string
	struct String str = create_string();
	
	printf("Enter \"quit\" to exit.\nType anything in the console!\n");
	
	// Loop until user types quit
	while(1){
		// Buffer to hold input
		char buffer[256];
		fgets(buffer,255,stdin);

		// Check if it's "quit"
		if(!strcmp(buffer,"quit\n")) break;

		// Append the input to the string
		append_string(str,"%s",buffer);
	}
	
	// Print the string
	printf("\n\n====| This is what you just typed: |====\n%s\n",str.str);
	
	// Allocate the memory
	free_string(str);

	return 0;
}
