#include <cstdio>
#include <vector>

using namespace std;

typedef long long ll;

const int NMAX = (int)1e5 + 5;
const ll INF = (ll)1e12;

struct edge {
	int from,to;
	ll w;
};

vector<edge> e;
ll dist[NMAX];

int main()
{
	struct edge cur_edge;
	int n,m;
	scanf("%d %d", &n, &m);
	for(int i = 1; i <= m; i++) {
		scanf("%d %d %lld", &cur_edge.from, &cur_edge.to, &cur_edge.w);
		e.push_back(cur_edge);
	}

	for(int i = 1; i <= n; i++)
		dist[i] = INF;
	dist[1] = 0;

	for(int i = 1; i <= n; i++) {
		bool changed = false;
		for(int j = 0; j < m; j++) {
			int from = e[j].from, to = e[j].to;
			if(dist[from] != INF && dist[from] + e[j].w < dist[to]) {
				dist[to] = dist[from] + e[j].w;
				changed = true;
			}
		}
		if(!changed)
			break;
	}

	for(int i = 1; i <= n; i++)
		printf("%lld ", dist[i]);
	printf("\n");

	return 0;
}


