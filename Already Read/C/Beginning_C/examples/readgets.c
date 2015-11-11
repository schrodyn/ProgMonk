#include <stdio.h>
#include <string.h>

int main(void){
	char initial[2] = {0};
	char name[80] = {0};

	printf("Enter your first initial: ");
	fgets(initial, sizeof(initial), stdin);
	
	int c;
	while((c = getchar()) != '\n' && c != EOF);

	printf("Enter your name: ");
	fgets(name, sizeof(name), stdin);
	size_t length = strlen(name);
	name[length-1] = '\0';

	if(initial[0] != name[0])
		printf("\n%s,you got your initial wrong.\n", name);
	else
		printf("\nHi, %s. Your initial is correct. Well done!\n", name);
	return 0;
}


