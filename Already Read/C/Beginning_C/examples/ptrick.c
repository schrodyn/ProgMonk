#include <stdio.h>

void modify(int **);

int main(void){
	int x = 10;
	const int *p = &x;
	modify(&p);

	printf("*p = %d\n", *p);
	return 0;
}

void modify(int ** p){
	**p = 11;
}
