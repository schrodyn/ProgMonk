#include <stdio.h>

int main(void){
	struct horse
	{
		int age;
		int height;
		char name[20];
		char father[20];
		char mother[20];
	};

	struct horse My_first_horse;

	printf("Enter the name of the horse: ");
	scanf("%s", My_first_horse.name);

	printf("How old is %s? ", My_first_horse.name);
	scanf("%d", &My_first_horse.age);
	
	printf("How high is %s ( in hands )? ", My_first_horse.name);
	scanf("%d", &My_first_horse.height);

	printf("Who is %s's father? ", My_first_horse.name);
	scanf("%s", My_first_horse.father);

	printf("Who is %s's mother? ", My_first_horse.name);
	scanf("%s", My_first_horse.mother);

	printf("\n%s is %d years old, %d hands high,",
		My_first_horse.name, My_first_horse.age, My_first_horse.height);
	printf(" and has %s and %s as parents.\n", My_first_horse.father,
						My_first_horse.mother);
	return 0;
}
