#ifndef CDS_ADVANCED_STRING_H
#define CDS_ADVANCED_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// An advanced string is, to put it simply, more than a character string
// It acts like a vector, but supports more complex behavior, such as:
//  - Formatting
//  - String literal concatenation
//  - String literal assignment

// Now, for formatting to be supported, its important to know that we
// need extra size for the new formatted elements to be added correctly.
// Here is the global variable holding that value:
size_t _STRING_APPEND = 20;
// Change it if you need more overhead, more space

// Advanced string structure
struct String{
	char* str;
	size_t size;
};

// Create a new string
// Here, the string starts with size=1
// That is because the appending will fail to allocate the extra byte for '\0' otherwise
#define create_string() (struct String){NULL,0};

// Free / clear a string
#define free_string(s) ({if((s).size) free((s).str); (s).str = NULL; (s).size = 0;})

// Set the string to another string that may be formatted
#define set_string(d,s,...) ({\
	free_string((d));\
	(d).str = malloc(sizeof(char)*((d).size = strlen((s))+_STRING_APPEND+1));\
	(d).size = sprintf((d).str,(s),##__VA_ARGS__);\
})

// Append a string to advanced string, may have a format
#define append_string(d,s,...) ({\
	(d).str = realloc((d).str,sizeof(char)*((d).size+strlen((s))+_STRING_APPEND+1));\
	(d).size += sprintf((d).str+(d).size,(s),##__VA_ARGS__);\
	(d).str[(d).size]='\0';\
})

// Allocate (n) bytes for string
#define alloc_string(d,n) ({\
	(d).size += (n)+1;\
	(d).str = realloc((d).str,sizeof(char)*(d).size);\
	(d).str[(d).size]='\0';\
})

#endif
