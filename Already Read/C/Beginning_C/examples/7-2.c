#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define NUM_P 100
#define BUFFER_SIZE 256

int main(void){
	char *pS[NUM_P];
	char buffer[BUFFER_SIZE];
	int count = 0;	

	printf("Enter proverbs, Empty enter to exit:\n");
	while((*fgets(buffer, BUFFER_SIZE, stdin) != '\n') && (count < NUM_P)){
		size_t in_length = strlen(buffer);
		buffer[--in_length]='\0';
		pS[count] = (char*)malloc(in_length+1);

		if(pS[count]==NULL){
			printf("Memory allocation failed.\n");
			break;
		}

		strcpy(pS[count++], buffer);
	}

	bool sorted = false;
	char *pTemp = NULL;
	while(!sorted){
		sorted = true;
		for(int i = 0; i < count-1; i++)
			if(strlen(pS[i]) > strlen(pS[i+1])){
				sorted = false;
				pTemp = pS[i];
				pS[i] = pS[i+1];
				pS[i+1] = pTemp;	
			}
	}
	printf("\nsorted proverbs:\n");		
	for(int i = 0; i < count; i++){
		printf("%s\n", pS[i]);
		free(pS[i]);
	}
	return 0;
		
}
