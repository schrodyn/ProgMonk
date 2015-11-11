#include <stdio.h>

long *IncomePlus(long *pPay);

int main(void){
	long your_pay = 30000L;
	long *pold_pay = &your_pay;
	long *pnew_pay = NULL;
	
	pnew_pay = IncomePlus( pold_pay );
	printf("\nOld pay = $%ld\tNew pay = $%ld\n", *pold_pay,  *pnew_pay);
	return 0;
}

long *IncomePlus(long *pPay){
	long pay = 0;
	pay = *pPay + 10000L;
	return &pay;
}
