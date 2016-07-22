#include <iostream>

using namespace std;

const int NMAX = (int)1e5 + 10;

int arr[NMAX];

void merge(int *arr, int start, int mid, int end)
{
	int p = start, q = mid+1;

	int arr1[end-start+1];
	int k = 0;

	for(int i = start; i <= end; i++){
		if(p > mid)
			arr1[k++] = arr[q++];
		else if(q > end)
			arr1[k++] = arr[p++];
		else 
			if(arr[p] < arr[q])
				arr1[k++] = arr[p++];
		else
			arr1[k++] = arr[q++];
			
	}

	for(int i = 0; i < k; i++)
		arr[start++] = arr1[i];
}

void merge_sort(int *arr, int start, int end)
{
	if(start < end) {
		int mid = (start + end) / 2;
		merge_sort(arr, start, mid);
		merge_sort(arr, mid+1, end);

		merge(arr, start, mid, end);
	}
}

int main()
{
	int t;
	int n;
	cin >> t;
	while(t--){
		cin >> n;
		for(int i = 0; i < n; i++)
			cin >> arr[i];

		merge_sort(arr, 0, n-1);
		
		for(int i = 0; i < n; i++)
			cout << arr[i] << " ";
		cout << endl;
	}
}
