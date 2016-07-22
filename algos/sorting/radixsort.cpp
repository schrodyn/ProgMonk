#include <cstdio>
#include <vector>
#include <algorithm>

/*
 * good description:
 * http://www.cprogramming.com/tutorial/computersciencetheory/radix.html
 */

using namespace std;

const int NMAX = (int)1e6 + 5;

int arr[NMAX];

void radix_sort(int *arr, int n)
{
	vector<int> buckets[10];
	int minNum, maxNum, power = 1;
	
	minNum = maxNum = arr[0];
	for(int i = 1; i < n; i++) {
		minNum = min(minNum, arr[i]);
		maxNum = max(maxNum, arr[i]);
	}
	
	for(int i = 0; i < n; i++)
		arr[i] -= minNum;
	
	while(true) {
		for(int i = 0; i < 10; i++)
			buckets[i].clear();
		
		for(int i = 0; i < n; i++) {
			int digit = (arr[i] / power) % 10;
			buckets[digit].push_back(arr[i]);
		}

		int k = 0;
		for(int i = 0; i < 10; i++)
			for(int j = 0; j < (int)buckets[i].size(); j++) {
				arr[k++] = buckets[i][j];
			}
	
		power *= 10;
		if(power > maxNum)
			break;
	}

	for(int i = 0; i < n; i++)
		arr[i] += minNum;
}


int main()
{
	int n;
	scanf("%d", &n);

	for(int i = 0; i < n; i++)
		scanf("%d", &arr[i]);
	
	radix_sort(arr, n);

	for(int i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");

}


