#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define LENGTH 50

void eatspaces(void);
bool getinteger(int *n);
char *getname(char *name, size_t length);
bool isnewline(void);

int main(void){
	int number;
	char name[LENGTH];
	printf("Enter a sequence of integers and alphabetic names:\n");
	while(!isnewline())
		if(getinteger(&number))
			printf("\nInteger value:%8d", number);
		else if(strlen(getname(name, LENGTH)) > 0)
			printf("\nName: %s", name);
		else
		{
			printf("\nInvalid input.");
			return 1;
		}
	return 0;
}

bool isnewline(void){
	char ch = 0;
	if((ch = getchar()) == '\n')
		return true;
	
	ungetc(ch, stdin);
	return false;
}

void eatspaces(void){
	char ch = 0;
	while(isspace(ch = getchar()));
	ungetc(ch, stdin);
}

bool getinteger(int *n){
	eatspaces();
	int value = 0;
	int sign = 1;
	char ch = 0;

	if((ch=getchar()) == '-')
		sign = -1;
	else if(isdigit(ch))
		value = 10*value + (ch - '0');
	else if(ch != '+')
	{
		ungetc(ch, stdin);
		return false;
	}

	while(isdigit(ch = getchar()))
		value = 10*value + (ch - '0');

	ungetc(ch, stdin);
	*n = value*sign;
	return true;	
}

char *getname(char *name, size_t length)
{
	eatspaces();
	size_t count = 0;
	char ch = 0;
	while(isalpha(ch=getchar()))
	{
		name[count++] = ch;
		if(count == length-1)
			break;
	}

	name[count] = '\0';
	if(count < length-1)
		ungetc(ch, stdin);
	return name;
}




