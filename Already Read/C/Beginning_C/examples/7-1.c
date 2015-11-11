#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_NUM 100
#define NUM_LEN 30
#define BUFFER_LEN 256

int main(void){
	double *pnums[MAX_NUM];
	char buffer[BUFFER_LEN];
	char number_string[NUM_LEN];
	int count = 0;
	
 	printf("Enter values:\n");
	while(strcmp(fgets(buffer, BUFFER_LEN, stdin), "quit\n") != 0
						&& count < MAX_NUM){
		size_t input_length = strlen(buffer);
		buffer[--input_length]='\0';
		int index = 0;
		int to = 0;
		for(index = 0; index <= input_length; index++) /* remove spaces */
		 	if(buffer[index] != ' ')
				buffer[to++] = buffer[index];
		input_length = strlen(buffer);
		index = 0;
		to = 0;
		for(; (index < input_length) && count < MAX_NUM; to = 0){	
			if(buffer[index]=='+' ||buffer[index]=='-')
				*(number_string+to++) = *(buffer+index++);
			for(; isdigit(buffer[index]); index++)
				*(number_string+to++) = *(buffer+index);

			if(buffer[index]=='.'){
				*(number_string+to++) = *(buffer+index++);
				for(; isdigit(buffer[index]); index++)
					*(number_string+to++) = *(buffer+index);
			}
			number_string[to] = '\0';
			pnums[count] = (double*)malloc(sizeof(double));
			*pnums[count++] = atof(number_string);	
		}
	}
	double res = 0;
	for(int index = 0; index < count; index++){
		res += *pnums[index];
	}
	printf("\nresult: %lf\n", res/count);
}
