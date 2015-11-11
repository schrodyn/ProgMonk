#include <stdio.h>

int main(void){
	long multiple[] = {15L, 25L, 35L, 45L};
	long *p = multiple;
	
	for(int i = 0; i < (int)sizeof(multiple)/sizeof(multiple[0]); i++)
		printf("\naddress p+%d (&multiple[%d]): %p *(p+%d) value: %ld",
				i,	i,	p+i,	i,	*(p+i));
	return 0;
}
