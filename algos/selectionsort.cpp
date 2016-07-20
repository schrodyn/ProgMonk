#include <cstdio>
#include <algorithm>

using namespace std;

const int NMAX = (int)1e6 + 5;

int arr[NMAX];

void selection_sort(int *arr, int n)
{
	int min;
	for(int i = 0; i < n; i++) {
		//find minimum element and swap with i-th element
		min = i;
		for(int j = i+1; j < n; j++) {
			if(arr[j] < arr[min])
				min = j;
		}

		swap(arr[min], arr[i]);

	}
}

int main()
{
	int n;
	scanf("%d", &n);

	for(int i = 0; i < n; i++)
		scanf("%d", &arr[i]); 
	
	selection_sort(arr, n);

	for(int i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
}


