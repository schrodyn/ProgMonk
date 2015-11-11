#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_LEN 256

void to_string(char* s, int n);
void strrev(char *p);

int main(void){
	int n;
	char s[BUFFER_LEN];
	printf("\nEnter a number to convert to string: ");
	scanf("%d", &n);
	to_string(s, n);
	printf("converted number is %s\n", s);
	return 0;
}

void to_string(char* s, int n){
	int index = 0;
	bool is_neg = false;
	if(n < 0){ 
	  is_neg = true;
	  n*=-1;
	}
	while(n){
		s[index++] = (n%10) + '0';
		n/=10;
	}
	if(is_neg)
		s[index++]='-';
	s[index]='\0';
	strrev(s);	
}

void strrev(char *p){
	char *q = p;
	while(q && *q) ++q;
	for(--q; p < q; p++, q--){
		*p = *p ^ *q;
		*q = *p ^ *q;
		*p = *p ^ *q;
	}
}

