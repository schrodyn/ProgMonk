#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main(void){
	struct horse
	{
		int age;
		int height;
		char name[20];
		char father[20];
		char mother[20];
		struct horse *next;
		struct horse *previous;
	};

	struct horse *first = NULL;
	struct horse *current = NULL;
	struct horse *last = NULL;

	char test = '\0';

	for( ; ; ){
		printf("\nDo you want to enter details of a%s horse (Y or N)",
				first == NULL?"nother":"");
		scanf(" %c", &test);
		if(tolower(test) == 'n')
			break;

		current = (struct horse*)malloc(sizeof(struct horse));

		if(first == NULL){
			first = current;
			current->previous = NULL;
		}
		else{
			last->next = current;
			current->previous = last;
		}

		printf("\nEnter the name of the horse: ");
		scanf("%s", current->name);

		printf("\nHow old is %s? ", current->name);
		scanf("%d", &current->age);

		printf("\nHow high is %s ( in hands )? ", current->name);
		scanf("%d", &current->height);

		printf("\nWho is %s's father? ", current->name);
		scanf("%s", current->father);

		printf("\nWho is %s's mother? ", current->name);
		scanf("%s", current->mother);

		current->next = NULL;
		last = current;
	}

	while(current != NULL){
		printf("\n\n%s is %d years old, %d hands high",
				current->name, current->age, current->height);
		printf(" and has %s and %s as parents.", current->father,
							 current->mother);
		last = current;
		current = current->previous;
		free(last);
	}
	return 0;
}
