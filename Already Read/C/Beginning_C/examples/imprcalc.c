#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#define BUFFER_LEN 256

int main(void){
	char input[BUFFER_LEN];
	char number_string[30];
	char op = 0;
	
	unsigned int index = 0;
	unsigned int to = 0;
	size_t input_length = 0;
	unsigned int number_length = 0;
	double result = 0.0;
	double number = 0.0;

	printf("Enter an expression:\n");
	
	while(strcmp(fgets(input, BUFFER_LEN, stdin), "quit\n") != 0){
		input_length = strlen(input);
		input[--input_length] = '\0';

		for(to = 0, index = 0; index <= input_length; index++)
			if(*(input+index) != ' ')
				*(input+to++) = *(input+index);

		input_length = strlen(input);
		index = 0;

		if(input[index] == '=')
			index++;
		else{
			number_length = 0;
			if(input[index]=='+' || input[index]=='-')
				*(number_string+number_length++) = *(input+index++);
			for( ; isdigit(*(input+index)); index++)
				*(number_string+number_length++) = *(input+index);
			
			if(*(input+index) == '.'){
				*(number_string+number_length++) = *(input+index++);
				for( ; isdigit(*(input+index)); index++)
					*(number_string+number_length++) = *(input+index);
			
			}
			*(number_string+number_length)='\0';
			if(number_length>0)
				result = atof(number_string);
		}

		for(; index < input_length;){
			op = *(input+index++);
			
			number_length = 0;
			if(input[index]=='+'|| input[index]=='-')
				*(number_string+number_length++) = *(input+index++);
			for(; isdigit(*(input+index)); index++)
				*(number_string+number_length++) = *(input+index);
			
			if(*(input+index)=='.'){
				*(number_string+number_length++) = *(input+index++);
				for(;isdigit(*(input+index)); index++)
					*(number_string+number_length++) = *(input+index);
			}
			*(number_string+number_length)='\0';
			number = atof(number_string);
			
			switch(op){
				case '+':
					result += number;
					break;
				case '-':
					result -= number;
					break;
				case '*':
					result *= number;
					break;
				case '/':
					if(number == 0)
						printf("Division by zero!\n");
					else
						result /= number;
					break;
				case '%':
					if((long)number == 0)
						printf("Division by zero!\n");
					else
						result=(double)((long)result % (long)number);
					break;
				case '^':
					result = pow(result, number);
					break;
				default:
					printf("\nIllegal operation!\n");
					break;
			}
		}
	printf("=%lf\n", result);			
	}
	return 0;
}
