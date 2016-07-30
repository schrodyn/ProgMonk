#include <cstdio>

const int NMAX = (int)1e5 + 5;

int arr[NMAX];
int size[NMAX];

int root(int s)
{
	int i = s;
	while(arr[i] != i) {
		arr[i] = arr[arr[i]];
		i = arr[i];
	}
	return i;
}


void unite(int a, int b)
{
	int root_a = root(a);
	int root_b = root(b);

	if(root_a == root_b)
		return;
	
	if(size[root_a] < size[root_b]) {
		arr[root_a] = root_b;
		size[root_b] += size[root_a];
	}
	else {
		arr[root_b] = root_a;
		size[root_a] += size[root_b];
	}
}

int main()
{
	int n,m,s1,s2,res = 0;
	scanf("%d %d", &n, &m);
	//initialize
	for(int i = 1; i <= n; i++) {
		arr[i] = i;
		size[i] = 1;
	}

	while(m--) {
		scanf("%d %d", &s1, &s2);
		unite(s1, s2);
	}

	for(int i = 1; i <= n ; i++) {
		if(arr[i] == i)
			res++;
	}

	printf("num of groups is %d\n", res);
	return 0;	
}


