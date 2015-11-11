#include <stdio.h>
#include <limits.h>
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>

int main(void){
	int count = 0;
	char ch = 0;
/*	printf("\nThe printable characters are the following:\n");
	
	for(int code = 0; code <= CHAR_MAX; code++)
	{
		ch = (char)code;
		if(isprint(ch))
		{
			if(count++ % 32 == 0)
				printf("\n");
			printf("%c", ch);
		}
	}
*/
	count = 0;
	wchar_t wch = 0;
	wprintf(L"\n\nThe alphabetic characters and their codes are the following:\n");
	
	for(wchar_t wch = L'a'; wch <= L'z'; wch++)
	{
		if(count++ % 3 == 0)
			wprintf(L"\n");
		
		wprintf(L" %lc %x %lc %#x", wch, (long)wch, towupper(wch),
								(long)towupper(wch));
	}
	wprintf(L"\n\n%lc %x %lc %#x\n", L'a', (long)L'a', towupper(L'a'), (long)towupper(L'a'));
	return 0;
}
