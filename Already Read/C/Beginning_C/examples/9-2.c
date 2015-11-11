#include <stdio.h>

double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double array_op(double array[], int size, double (*pfun)(double, double));

int main(void){
	double arr[] = {1,2,3,4};
	printf("array_op -> arr -> 4 -> add = %lf\n", array_op(arr, 4, add));	
	printf("array_op -> arr -> 4 -> subtract = %lf\n", array_op(arr, 4, subtract));	  
	printf("array_op -> arr -> 4 -> multiply = %lf\n", array_op(arr, 4, multiply));
	return 0;	
}

double array_op(double array[], int size, double (*pfun)(double, double)){
	double res = 0;
	if(pfun != subtract){	
		if(pfun == multiply)
			res = 1;
		for(int i = 0; i < size; i++){
			res = pfun(res, array[i]);
		}
	}else
	{
		int neg = -1;
		for(int i = 0; i < size; i++){
			res = subtract(res, neg*array[i]);
			neg *= -1;
		}	
	}	
	return res;
}

double add(double a, double b){
	return a+b;
}

double multiply(double a, double b){
	return a*b;
}

double subtract(double a, double b){
	return a-b;
}
