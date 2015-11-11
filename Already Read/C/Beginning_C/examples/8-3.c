#include <stdio.h>
#include <stdbool.h>
#define BUFFER_LEN 256

void to_string(char *s, int n, int count);
void strrev(char *p);

int main(void){
	int n, count;
	char s[BUFFER_LEN];
	printf("enter a number to be converted: ");
	scanf("%d", &n);
	printf("enter a count of digits: ");
	scanf("%d", &count);
	to_string(s, n, count);
	printf("result string: %s\n", s);
}

void to_string(char *s, int n, int count){
	int index = 0;
	bool is_neg = false;
	if (n < 0){
		is_neg = true;
		n *= -1;
	}
	while(n){
		s[index++] = (n%10) + '0';
		n /= 10;
	}
	if(is_neg)
		s[index++]='-';
	while(index < count)
		s[index++]=' ';
	s[index]='\0';
	strrev(s);
}

void strrev(char *p){
	char *q = p;
	while(q && *q) q++;
	for(--q; p < q; p++, q--){
		*p = *p ^ *q;
		*q = *p ^ *q;
		*p = *p ^ *q;
	}
}
