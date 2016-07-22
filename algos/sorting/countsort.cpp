#include <cstdio>

/* visualization: 
https://www.cs.usfca.edu/~galles/visualization/CountingSort.html
*/

const int NMAX = (int)1e5 + 6;

int arr[NMAX];
int sorted[NMAX];
int count[NMAX];

void count_sort(int *arr, int *out, int n)
{
	for(int i = 0; i < NMAX; i++)
		count[i] = 0;
	
	for(int i = 0; i < n; i++)
		count[arr[i]]++;
	
	for(int i = 1; i < NMAX; i++)
		count[i] += count[i-1];
	
	for(int i = 0; i < n; i++) {
		count[arr[i]]--;
		out[count[arr[i]]] = arr[i];
	}
}

// count sort without second array 
void count_sort2(int *arr, int n)
{
	for(int i = 0; i < NMAX; i++)
		count[i] = 0;
	
	for(int i = 0; i < n; i++)
		count[arr[i]]++;
	
	int b = 0;
	for(int i = 0; i < NMAX; i++) {
		for(int j = 0; j < count[i]; j++) {
			arr[b++] = i;
		}
	}
}

int main()
{
	int n;
	scanf("%d", &n);
	for(int i = 0; i < n; i++)
		scanf("%d", &arr[i]);
	
	//count_sort(arr, sorted, n);
	count_sort2(arr, n);
	
	for(int i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

