#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

char *to_string(int count, double first,...);

int main(void){
	printf("to_string(4,1,2,3.456675566,9) = %s\n", 
			to_string(4,1.0,2.0,3.456675566,9.0));
	printf("to_string(3,4,5,6) = %s\n", to_string(3,4.0,5.0,6.0));
	return 0;
}


char *to_string(int count, double first,...){
	va_list pargs;
	char *s = (char *)malloc(16*count);
	char arg_buffer[16];
	snprintf(arg_buffer, 16, "%.2lf", first);
	arg_buffer[strlen(arg_buffer)+1]='\0';
        arg_buffer[strlen(arg_buffer)]=',';
        strcpy(s, arg_buffer); 

	va_start(pargs, first);
	double arg = 0;
	for(int i = 1; i < count; i++){
		arg = va_arg(pargs, double);
		snprintf(arg_buffer, 16, "%.2lf", arg);
		arg_buffer[strlen(arg_buffer)+1]='\0';
		arg_buffer[strlen(arg_buffer)]=',';
		strcat(s, arg_buffer);			
	}
	s[strlen(s)-1]='\0';
	va_end(pargs);
	return s;
}





