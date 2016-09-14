#include <cstdio>
#include <cmath>

using namespace std;

const int NMAX = (int)1e6 + 5;

int arr[NMAX];

/* 
 increment sequence is Mersenne numbers (2^i - 1).  
 Also called Hibbard's increments. 
 With this increment sequence, it's O(n^(3/2))
*/
void shell_sort_hibbard(int *arr, int n)
{
	int j,h;

	// determine the increment sequence - mersenne numbers (2^x - 1)
	for(h = 1; h < n; h = (h+1)*2 - 1);	
	h = (h+1)/2 - 1;
	
	while(h > 0) {

		for(int i = h; i < n; i++) {
			
			int tmp = arr[i];
			
			for(j = i; j-h >= 0 && tmp < arr[j-h]; j-=h) {
				arr[j] = arr[j-h];
			}
			arr[j] = tmp;
		}
		h = (h+1)/2 - 1;
	}
}
/*
 * Knuth's increment sequence h = 3*h+1
 * This is also O(n^(3/2))))
 */
void shell_sort_knuth(int *arr, int n)
{
	int j,h;

	for(h = 1; h < n; h = 3*h+1);
	h/=3;

	while(h > 0) {

		for(int i = h; i < n; i++) {

			int tmp = arr[i];

			for(j = i; j-h >= 0 && tmp < arr[j-h]; j-=h) {
				arr[j] = arr[j-h];
			}
			arr[j] = tmp;
		}
		h/=3;
	}
}

int main()
{
	int n;
	scanf("%d", &n);

	for(int i = 0; i < n; i++)
		scanf("%d", &arr[i]);
	
	shell_sort_hibbard(arr, n);

	for(int i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

