#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

typedef vector<int> vi;

const int NMAX = (int)1e5 + 5;

//int arr[NMAX];
vi arr;

int binary_search(int *arr, int l, int r, int item)
{	
	while(l <= r) {
		int m = l + (r - l)/2;
		
		if(arr[m] == item)
			return m;

		if(arr[m] < item)
			l = m+1;
		else
			r = m-1;
	}

	return -1;
}

int bin_search_vec(vi &arr, int l, int r, int item)
{
	while(l <= r) {
		int m = l + (r - l)/2;
		
		if(arr[m] == item)
			return m;

		if(arr[m] < item)
			l = m+1;
		else
			r = m-1;
	}

	return -1;
}

int main()
{
	int n,t, num;
	cin >> n;
	arr.reserve(n);
	
	for(int i = 0;i < n; i++)
		cin >> arr[i];
	
	cin >> t;

	//sort(arr, arr + n);

	sort(arr.begin(), arr.begin()+n);

	for(int i = 0; i < n; i++)
		cout << arr[i] << endl; 

	//printf("t element position is %d\n", binary_search(arr, 0, n - 1, t));
	printf("t element position is %d\n", bin_search_vec(arr, 0, n - 1, t));
}
