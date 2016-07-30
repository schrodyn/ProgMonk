#include <cstdio>
#include <vector>
#include <queue>
#include <utility>
#include <functional>

using namespace std;

const int NMAX = (int)1e5 + 5;

typedef long long ll;
typedef pair<long long, int> pll;

vector<pll> g[NMAX];
bool used[NMAX];

ll prim(int s)
{
	ll min_cost = 0;
	pll p;
	priority_queue<pll, vector<pll>, greater<pll> > q;
	
	q.push(make_pair(0,s));
	while(!q.empty()) {
		//select edge with min cost
		p = q.top();
		q.pop();
		int v = p.second;

		//check for cycle
		if(used[v])
			continue;

		min_cost += p.first;
		used[v] = true;

		//add all childs
		for(int i = 0; i < g[v].size(); i++) {
			int to = g[v][i].second;
			if(!used[to])
				q.push(g[v][i]);
		}

	}
	return min_cost;
}

int main()
{
	int n,edges,x,y;
	ll c,res;
	scanf("%d %d", &n, &edges);

	for(int i = 0; i < edges; i++) {
		scanf("%d %d %lld", &x, &y, &c);
		g[x].push_back(make_pair(c,y));
		g[y].push_back(make_pair(c,x));
	}

	res = prim(1);

	printf("min cost is %lld\n", res);
	return 0;
}



