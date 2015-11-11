#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int LENGTH = 80;

int main(void){
	char *proverbs[] = { "Many a mickle makes a muckle.\n",
			     "Too many cooks spoil the broth.\n",
			     "He who laughs last didn't get the joke in"
							" the first place.\n"
			};
	char more[LENGTH];
	FILE *pfile = NULL;
	char *filename = "myfile";

	if(!(pfile = fopen(filename, "w")))
	{
		printf("Error opening %s for writing. Program terminated.", filename);
		exit(1);
	}

	int count = sizeof proverbs / sizeof proverbs[0];
	for(int i = 0; i < count; i++)
	  fputs(proverbs[i], pfile);

	fclose(pfile);

	if(!(pfile = fopen(filename, "a")))
	{
		printf("Error opening %s for writing. Program terminated.", filename);
		exit(1);
	}

	printf("Enter proverbs of less than 80 characters or press Enter to end:\n");
	while(true)
	{
		fgets(more, LENGTH, stdin);
		if(more[0] == '\n')
			break;
		fputs(more, pfile);
	}

	fclose(pfile);

	if(!(pfile = fopen(filename, "r")))
	{
		printf("Error opening %s for reading. Prgram terminated.", filename);
		exit(1);
	}

	printf("The proverbs in the file are:\n\n");
	while(fgets(more, LENGTH, pfile))
		printf("%s", more);

	fclose(pfile);
	//remove(filename);
	return 0;	
}
