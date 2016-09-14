#include <cstdio>
#include <algorithm>

using namespace std;

const int NMAX = (int)1e6 + 5;

int arr[NMAX];

/* find the first occurrence of a given input item */
int binsearch_first(int *arr, int l, int r, int item)
{
	while(r - l > 1) {
		int m = l + (r-l)/2;

		if(arr[m] >= item)
			r = m;
		else
			l = m;
	}

	return r;
}

/* find the last occurrence of a given input item */
int binsearch_last(int *arr, int l, int r, int item)
{
	while(r - l > 1) {
		int m = l + (r-l)/2;

		if(arr[m] <= item)
			l = m;
		else
			r = m;
	}

	return l;
}

int num_of_occurrences(int *arr, int n, int item)
{
	int left = binsearch_first(arr, -1, n-1, item);
	int right = binsearch_last(arr, 0, n, item);
	
	if(arr[left] == item && arr[right] == item) //if item is in array
		return (right - left + 1);
	
	return 0;
}

int main()
{
	int n,t;
	scanf("%d", &n);

	for(int i = 0; i < n; i++)
		scanf("%d", &arr[i]);
	
	sort(arr, arr+n);

	scanf("%d", &t);

	printf("t first position is %d\n", binsearch_first(arr, -1, n-1, t));
	printf("t last position is %d\n", binsearch_last(arr, 0, n, t));	
	printf("t number of occurences is %d\n", num_of_occurrences(arr, n, t));
}

