#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;

const int NMAX = (int)1e6 + 5;

vector<int> g[NMAX];
bool used[NMAX];

void bfs(int s)
{
	queue<int> q;
	q.push(s);
	used[s] = true;

	while(!q.empty()) {

		int v = q.front();
		q.pop();

		for(int i = 0; i < (int)g[v].size(); i++) {
			int to = g[v][i];
			if(!used[to]) {
				q.push(to);
				used[to] = true;
			}
		}

	}
}

int main()
{
	int nodes, edges, x, y, components = 0;
	scanf("%d %d", &nodes, &edges);

	memset(used, 0, sizeof(bool) * (nodes+1));

	for(int i = 0; i < edges; i++) {
		scanf("%d %d", &x, &y);
		g[x].push_back(y);
		g[y].push_back(x);
	}

	for(int i = 1; i <= nodes; i++)
		if(!used[i]) {
			bfs(i);
			components++;
		}
	
	printf("%d\n", components);
}



