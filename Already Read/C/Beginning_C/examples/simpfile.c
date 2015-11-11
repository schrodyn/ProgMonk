#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int LENGTH = 80;

int main(void){
	char mystr[LENGTH];
	int lstr = 0;
	int mychar = 0;
	FILE *pfile = NULL;
	char *filename = "myfile";
	printf("\nEnter an interesting string of less than 80 characters:\n");
	fgets(mystr, LENGTH, stdin);

	if(!(pfile = fopen(filename, "w")))
	{
		printf("Error opening %s for writing. Program terminated.", filename);
		exit(1);
	}

	lstr = strlen(mystr);
	for(int i = lstr-1; i >= 0; i--)
		fputc(mystr[i], pfile);

	fclose(pfile);

	if(!(pfile = fopen(filename, "r")))
	{
		printf("Error opening %s for reading. Program terminated.", filename);
		exit(1);
	}

	while((mychar = fgetc(pfile)) != EOF)
		putchar(mychar);
	putchar('\n');

	fclose(pfile);
	remove(filename);
	return 0;
}
