#include <stdio.h>
#include <stdlib.h>

int main(void){
	long num1 = 234567L;
	long num2 = 345123L;
	long num3 = 789234L;

	long num4 = 0L;
	long num5 = 0L;
	long num6 = 0L;

	float fnum = 0.0f;
	int ival[6] = { 0 };
	FILE *pfile = NULL;
	char *filename = "myfile";

	if(!(pfile = fopen(filename, "w")))
	{
		printf("Error opening %s for writing. Program terminated.", filename);
		exit(1);
	}
	fprintf(pfile, "%6ld%6ld%6ld", num1, num2, num3);
	fclose(pfile);
	printf("\n %6ld %6ld %6ld", num1, num2, num3);

	if(!(pfile = fopen(filename, "r")))
	{
		printf("Error opening %s for reading. Program terminated.", filename);
		exit(1);
	}
	
	rewind(pfile);
	fscanf(pfile, "%2d%3d%3d%3d%2d%2d%3f", &ival[0], &ival[1],
				&ival[2], &ival[3], &ival[4], &ival[5], &fnum);
	fclose(pfile);
	remove(filename);

	printf("\n");
	for(int i = 0; i < 6; i++)
		printf("%sival[%d] = %d", i == 4? "\n\t" : "\t", i, ival[i]);
	printf("\nfnum = %f\n", fnum);
	return 0;
}
