#include <cstdio>
#include <algorithm>

using namespace std;

const int NMAX = (int)1e6 + 5;

int arr[NMAX];

void insertion_sort(int *arr, int n)
{

	for(int i = 0; i < n; i++) {
		//current element
		int tmp = arr[i];
		int j = i;

		while(j > 0 && tmp < arr[j-1]) {
			arr[j] = arr[j-1];
			j--;
		}
		//move current element to its correct position
		arr[j] = tmp;
	}
}

int main()
{
	int n;
	scanf("%d", &n);
	for(int i = 0; i < n; i++)
		scanf("%d", &arr[i]);
	
	insertion_sort(arr, n);

	for(int i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
}


