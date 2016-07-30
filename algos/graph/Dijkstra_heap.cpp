#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

typedef long long ll;

const ll INF = (ll)1e12;
const int NMAX = (int)1e5 + 5;

struct edge {
	int to;
	ll w;
};

vector<edge> g[NMAX];
ll dist[NMAX];
int p[NMAX];

priority_queue<pair<ll, int> > q;
edge e;
vector<int> ans;

int main()
{
	int n,m;
	scanf("%d %d", &n, &m);
	for(int i = 1; i <= m; i++) {
		int a,b;
		ll w;
		scanf("%d %d %lld", &a, &b, &w);
		e.to = b, e.w = w;
		g[a].push_back(e);
		e.to = a;
		g[b].push_back(e);
	}
	
	//init distances
	q.push(make_pair(0, 1));
	for(int i = 2; i <= n; i++) {
		dist[i] = INF;
	}

	while(!q.empty()) {
		int v = q.top().second;
		ll cur_dist = -q.top().first;
		q.pop();
		if(cur_dist > dist[v])
			continue;
		for(int i = 0; i < g[v].size(); i++) {
			int to = g[v][i].to;
			ll w = g[v][i].w;
			if(cur_dist + w < dist[to]) {
				dist[to] = cur_dist + w;
				p[to] = v;
				q.push(make_pair(-dist[to], to));
			}
		}
	}

	if(dist[n] == INF) {
		printf("-1");
		return 0;
	}

	int cur = n;
	while(p[cur] != 0) {
		ans.push_back(cur);
		cur = p[cur];
	}
	ans.push_back(1);

	reverse(ans.begin(), ans.end());
	for(int i = 0; i < (int)ans.size(); i++)
		printf("%d ", ans[i]);
	printf("\n");

	return 0;
}


