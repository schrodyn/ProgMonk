#include <stdio.h>

int sum(int, int);
int product(int, int);
int difference(int, int);

int main(void){
	int a = 10;
	int b = 5;
	int result = 0;
	int (*pfunc[3])(int, int);

	pfunc[0] = sum;
	pfunc[1] = product;
	pfunc[2] = difference;

	for(int i = 0; i < 3; i++){
		result = pfunc[i](a, b);
		printf("\nresult = %d", result);
	}

	result = pfunc[1](pfunc[0](a, b), pfunc[2](a, b));
	printf("\n\nThe product of the sum and difference = %d\n", result);

	return 0;
}

int sum(int a, int b){
	return a+b;
}

int product(int a, int b){
	return a*b;
}

int difference(int a, int b){
	return a-b;
}
