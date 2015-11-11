#include <stdio.h>

int main(void){
	int k = 678;

	printf("%%d %%o %%x %%X");
	printf("\n%d %o %x %X", k, k, k, k);

	printf("\n\n%%8d	%%-8d	%%+8d	%%08d	%%-+8d");
	printf("\n%8d %-8d %+8d %08d %-+8d", k, k, k, k, k);
	return 0;
}
