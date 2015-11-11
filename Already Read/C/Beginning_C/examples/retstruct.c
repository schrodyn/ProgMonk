#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

struct Family *get_person(void);

struct Date
{
	int day;
	int month;
	int year;
};

struct Family
{
	struct Date dob;
	char name[20];
	char father[20];
	char mother[20];
	struct Family *next;
	struct Family *previous;
};

int main(void){
	struct Family *first = NULL;
	struct Family *current = NULL;
	struct Family *last = NULL;
	char more = '\0';

	for( ; ; )
	{
		printf("\nDo you want to enter details of a%s person (Y or N)? ",
							first != NULL?"nother":"");
		scanf(" %c", &more);
		if(tolower(more) == 'n')
			break;
		
		current = get_person();
		
		if(first == NULL){
			first = current;
			last = current;
		}
		else{
			last->next = current;
			current->previous = last;
			last = current;
		}
	}

	while(current != NULL){
		printf("\n%s was born %d/%d/%d, and has %s and %s as parents.",
				current->name, current->dob.day,
				current->dob.month, current->dob.year,
				current->father, current->mother);

		last = current;
		current = current->previous;
		free(last);
	}
	return 0;
}

struct Family *get_person(void){
	struct Family *temp;

	temp = (struct Family*)malloc(sizeof(struct Family));
	printf("\nEnter the name of the person: ");
	scanf("%s", temp->name);

	printf("\nEnter %s's date of birth (day month year); ", temp->name);
	scanf("%d %d %d", &temp->dob.day, &temp->dob.month, &temp->dob.year);

	printf("\nWho is %s's father? ", temp->name);
	scanf("%s", temp->father);

	printf("\nWho is %s's mother? ", temp->name);
	scanf("%s", temp->mother);

	temp->next = temp->previous = NULL;
	return temp;
}



