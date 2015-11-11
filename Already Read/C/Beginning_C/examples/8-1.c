#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_MAX 100
#define NUM_LEN 30
#define BUFFER_LEN 256

double average(double *pnum, int count);

int main(void){
	double num[NUM_MAX];
	char buffer[BUFFER_LEN];
	char number[NUM_LEN];
	int count = 0;
	printf("enter numbers through space\n"
			"you can also press Enter; just press Enter to end: ");
	
	while(*fgets(buffer, BUFFER_LEN, stdin)!= '\n'){
		buffer[strlen(buffer)-1]='\0';
		int index = 0;
		int t = 0;
		while(buffer[index])
			if(buffer[index] != ' ')
				number[t++] = buffer[index++];
			else{
				if(count < NUM_MAX)
					num[count++] = atof(number);
				t = 0;
				index++;
			}
		
		num[count++] = atof(number);	
		
	}
	printf("average is %lf\n", average(num, count));	
}

double average(double *pnum, int count){
	double res = 0;
	for(double *p = pnum; p - pnum < count; p++)
		res += *p;
	return res/count;
}
