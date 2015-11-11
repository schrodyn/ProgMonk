#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 256

char *getString(char *buffer, char end_char);

int main(void){
	char buff[BUFFER_SIZE];
	printf("Enter the string: ");
	fgets(buff, BUFFER_SIZE, stdin);
	printf("\nTruncated string: %s", getString(buff, ':'));
}

char *getString(char *buffer, char end_char){
	char *p = buffer;
	while((*p != end_char) && (p - buffer < strlen(buffer))) p++;
	*p = '\0';
	return buffer;
}
