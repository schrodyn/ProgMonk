#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

const int NMAX = (int)1e6 + 5;

vector<int> g[NMAX];
bool used[NMAX];

void dfs(int v)
{
	used[v] = true;
	for(int i = 0; i < g[v].size(); i++)
		if(!used[g[v][i]])
			dfs(g[v][i]);
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
			dfs(i);
			components++;
		}
	
	printf("%d\n", components);
}



