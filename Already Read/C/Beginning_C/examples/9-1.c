#include <stdio.h>

double power(double x, int n);

int main(void){
	double x = 0;
	int n = 1;
	printf("Enter x and n: ");
	scanf("%lf %d", &x, &n);

	printf("\n%lf ^ %d = %lf\n", x, n, power(x,n));
}

double power(double x, int n){
	if (n==1)
		return x;
	else
		return x*power(x, n-1);
}
