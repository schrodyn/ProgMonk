#include <stdio.h>

int main(void){
	int i = 0;
	int j = 0;
	int k = 0;
	int n = 0;

	printf("Input:\n");
	n = scanf(" %d %x %o", &i, &j, &k);

	printf("\nOutput:\n");
	printf("%d values read.", n);
	printf("\ni = %d\tj = %X\tk = %d", i, j, k);
	return 0;
}