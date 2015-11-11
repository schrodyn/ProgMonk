#include <stdio.h>

unsigned long factorial(unsigned long);

int main(void){
	unsigned long number = 0L;
	scanf("%lu", &number);
	printf("\nThe factorial of %lu is %lu\n", number, factorial(number));
	return 0;
}

unsigned long factorial(unsigned long n){
	if(n < 2L)
		return n;

	return n*factorial(n-1);
}
