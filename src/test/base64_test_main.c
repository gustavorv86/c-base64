
#include <stdio.h>
#include <stdlib.h>

#include "base64.h"

#define STR_SIZE 1024

int main() {

	char * orig;
	char * dest;
	
	orig = calloc(STR_SIZE, sizeof(char));
	
	strncpy(orig, "Hello, this is a BASE64 test example.", STR_SIZE);
	
	dest = base64_encode(orig, strlen(orig));
	free(orig);
	
	printf("encode (%ld): '%s'\n", strlen(dest), dest);
	
	orig = base64_decode(dest, strlen(dest));
	free(dest);
	
	printf("decode (%ld): '%s'\n", strlen(orig), orig);
	free(orig);
	
	return (EXIT_SUCCESS);
}

