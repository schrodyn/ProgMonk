#include <iostream>

using namespace std;

const int NMAX = (int)1e5 + 5;

int arr[NMAX];

int partition(int *arr, int start, int end)
{
	int i = start + 1;
	int piv = arr[start];

	for(int j = start + 1; j <= end; j++) {
		if(arr[j] < piv){
			swap(arr[i], arr[j]);
			i++;
		}
	}

	swap(arr[start], arr[i-1]);
	return i - 1;
}

void qsort(int *arr, int l, int r)
{
	if(l < r){
		int piv = partition(arr, l, r);
		qsort(arr, l, piv - 1);
		qsort(arr, piv + 1, r);
	}
}

int main() 
{
	int t;
	int n;
	cin >> t;

	while(t--){
	cin >> n;
	for(int i = 0; i < n; i++){
		cin >> arr[i];
	}
	qsort(arr, 0, n-1);

	for(int i = 0; i < n; i++)
		cout << arr[i] << std::endl;
	}
}
