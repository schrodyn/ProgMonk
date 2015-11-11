#include <stdio.h>

int count = 0;

void test1(void);
void test2(void);

int main(void){
	int count = 0;
	
	for( ; count < 5; count++){
		test1();
		test2();
	}
	return 0;
}

void test1(void){
	printf("\ntest1 count = %d", ++count);
}

void test2(void){
	static int count;
	printf("\ntest2 count = %d", ++count);
}
