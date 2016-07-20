#include <cstdio>
#include <algorithm>

using namespace std;

const int NMAX = (int)1e6 + 5;

int arr[NMAX];

void bubble_sort(int *arr, int n)
{
	for(int k = 0; k < n-1; k++)
		for(int i = 0; i < n-k-1; i++)
			if(arr[i] > arr[i+1])
				swap(arr[i], arr[i+1]);
}

int main()
{
	int n;
	scanf("%d", &n);
	for(int i = 0; i < n; i++)
		scanf("%d", &arr[i]);
	
	bubble_sort(arr, n);

	for(int i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");

}
