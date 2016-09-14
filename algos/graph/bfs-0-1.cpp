#include <cstdio>
#include <deque>
#include <vector>
#include <utility>

using namespace std;

const int NMAX = (int)1e6 + 5;

int d[NMAX];
vector<pair<int, int> > g[NMAX];

void bfs(int start)
{
	deque<int> q;
	q.push_back(start);
	d[start] = 0;

	while(!q.empty()) {

		int v = q.front();
		q.pop_front();

		for(int i = 0; i < (int)g[v].size(); i++) {
			if(d[g[v][i].first] > d[v] + g[v][i].second) {

				d[g[v][i].first] = d[v] + g[v][i].second;
				
				if(g[v][i].second == 0)
					q.push_front(g[v][i].first);
				else
					q.push_back(g[v][i].first);
			}
		}
	}
}

int main()
{
	int edges,x,y,c,t;

	scanf("%d", &edges);
	for(int i = 0; i < edges; i++) {
		scanf("%d %d %d", &x, &y, &c);
		g[x].push_back(make_pair(y,c));
		g[y].push_back(make_pair(x,c));
	}

	for(int i = 0; i < NMAX; i++)
		d[i] = NMAX;

	bfs(1);

	scanf("%d", &t);
	while(t--) {
		scanf("%d", &x);
		printf("%d\n", d[x]);
	}
}


