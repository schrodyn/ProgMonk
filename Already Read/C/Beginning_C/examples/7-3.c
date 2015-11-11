#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_LEN 256

int main(void){
	char buffer[BUFFER_LEN];
	printf("Enter string to split bastard:\n");
	fgets(buffer, BUFFER_LEN, stdin);
	size_t in_size = strlen(buffer);
	int to = 0;
	for(int i = 0; i < in_size; i++)
		if(isalnum(*(buffer+i)))
			*(buffer+to++) = *(buffer+i);
	printf("truncated string is: %s\n", buffer);
	return 0;
}
