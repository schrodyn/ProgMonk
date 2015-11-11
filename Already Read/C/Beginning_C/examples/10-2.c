#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

double read_double(void);

int main(void){
	double total = 0;
	int count = 0;
	char ch = 0;
	printf("Enter prices for b1g business:\n");
	while((ch = getchar()) != '\n'){
		if(ch == ',' || isspace(ch))
			continue;
		if(ch == '$')
			total += read_double();	
	}
	printf("\nTotal price = $%.2lf\n", total);
}

double read_double(void){
	static char num[10];
	int i = 0;
	while(!isspace(num[i++] = getchar()) && (i < 10));
	num[i]='\0';
	return atof(num);
}




