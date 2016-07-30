#include <cstdio>
#include <algorithm>

using namespace std;

const int NMAX = (int)1e5 + 5;

int arr[NMAX];

void heapify(int i, int size)
{
	int left = 2*i;
	int right = 2*i+1;

	int max = i;

	if(left <= size && arr[left] >= arr[max])
		max = left;
	
	if(right <= size && arr[right] >= arr[max])
		max = right;
	
	if(max != i) {
		swap(arr[i], arr[max]);
		heapify(max, size);
	}
}

void build_heap(int size)
{
	for(int i = size/2; i >= 1; i--)
		heapify(i, size);
}

void heap_sort(int n)
{
	build_heap(n);
	while(n) {
		swap(arr[1], arr[n]);
		n--;
		heapify(1, n);
	}
}

int main()
{
	int n;
	scanf("%d", &n);

	for(int i = 1; i <= n; i++)
		scanf("%d", &arr[i]);
	
	heap_sort(n);

	for(int i = 1; i <= n; i++)
		printf("%d ", arr[i]);
	printf("\n");

}


