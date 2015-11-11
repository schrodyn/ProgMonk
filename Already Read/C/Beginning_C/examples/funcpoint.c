#include <stdio.h>

int sum(int, int);
int product(int, int);
int difference(int, int);

int main(void){
	int a = 10;
	int b = 5;
	int result = 0;
	int (*pfunc) (int, int);

	pfunc = sum;
	result = pfunc(a, b);
	printf("\npfunc = sum\tresult = %d", result);

	pfunc = product;
	result = pfunc(a, b);
	printf("\npfunc = product\tresult = %d", result);

	pfunc = difference;
	result = pfunc(a, b);
	printf("\npfunc = difference\tresult = %d", result);
	return 0;
}

int sum(int x, int y){
	return x + y;
}

int product(int x, int y){
	return x * y;
}

int difference(int x, int y){
	return x - y;
}


