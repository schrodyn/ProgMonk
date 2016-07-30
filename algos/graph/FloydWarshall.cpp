#include <cstdio>

typedef long long ll;

const int NMAX = 105;
const ll INF = (int)1e12;

ll d[NMAX][NMAX];

int main()
{
	int n,m,from,to;
	ll w;
	scanf("%d %d", &n, &m);

	for(int i = 1; i <= n; i++)
		for(int j = 1; j <= n; j++)
			d[i][j] = INF;
	
	for(int i = 1; i <= m; i++) {
		scanf("%d %d %lld", &from, &to, &w);
		d[from][to] = w;
	}

	for(int k = 1; k <= n; k++) 
		for(int i = 1; i <= n; i++)
			for(int j = 1; j <= n; j++)
				if(d[i][k] + d[k][j] < d[i][j])
					d[i][j] = d[i][k] + d[k][j];
	
	int s,t;
	scanf("%d %d", &s, &t);
	if(d[s][t] == INF)
		printf("-1\n");
	else
		printf("%lld\n", d[s][t]);
}


