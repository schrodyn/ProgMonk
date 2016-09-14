#include <cstdio>
#include <algorithm>

using namespace std;

typedef long long ll;

struct edge {
	int v1;
	int v2;
	ll c;
};

const int NMAX = (int)1e5 + 5;

struct edge e[NMAX];
int arr[NMAX];
int size[NMAX];

bool cmp(const struct edge &e1, const struct edge &e2) 
{
	return e1.c < e2.c;
}

int root(int i)
{
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
	int n,edges;
	ll min_cost = 0;
	scanf("%d", &n);

	for(int i = 1; i <= n; i++) {
		arr[i] = i;
		size[i] = 1;
	}
	
	scanf("%d", &edges);
	for(int i = 0; i < edges; i++) {
		scanf("%d %d %lld", &e[i].v1, &e[i].v2, &e[i].c);
	}


	//kruskal
	sort(e, e + edges, cmp);
	for(int i = 0; i < edges; i++) {	
		if(root(e[i].v1) != root(e[i].v2)) {
			min_cost += e[i].c;
			unite(e[i].v1, e[i].v2);
		}
	}

	printf("min cost is %lld\n", min_cost);

}
