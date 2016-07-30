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

	if(left <= size && arr[left] > arr[max]) 
		max = left;
	
	if(right <= size && arr[right] > arr[max])
		max = right;
	
	if(max != i) {
		swap(arr[i], arr[max]);
		heapify(max, size);
	}
		
}

void insert(int s, int &size)
{
	arr[++size] = s;
	int i = size;
	while(i > 1 && arr[i] > arr[i/2]) {
		swap(arr[i], arr[i/2]);
		i /= 2;
	}
}

int get_front(int &size)
{
	int res = arr[1];
	arr[1] = arr[size--];
	heapify(1, size);
	return res;
}

int main()
{
	int t,n,size= 0;
	scanf("%d", &t);
	while(t--) {
		scanf("%d", &n);
		insert(n, size);
		printf("top is %d\n", arr[1]);
	}
}


