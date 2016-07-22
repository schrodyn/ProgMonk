#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

const int NMAX = (int)1e7 + 1;

/* array of least prime divisor */
int lp[NMAX];
int rest[NMAX];
vector<int> pr;

/*
 * print YES if number is prime
 * else print NO and number prime factorization
 */

int main()
{
	memset(lp, 0, sizeof(int)*NMAX);
	for(int i = 2; i <= NMAX; i++) {
		if(lp[i] == 0) {
			lp[i] = i;
			rest[i] = 1;
			pr.push_back(i);
		}
		for(int j = 0; j < (int)pr.size() && pr[j] <= lp[i] && i * 1ll * pr[j] <= NMAX; j++) {
			lp[i * pr[j]] = pr[j];
			rest[i * pr[j]] = i;
		}
	}

	int t,n;
	scanf("%d", &t);
	while(t--) {
		scanf("%d", &n);
		if(lp[n] == n)
			printf("YES\n");
		else {
			printf("NO\n");
			while(lp[n] != n) {
				printf("%d ", lp[n]);
				n = rest[n];
			}
			printf("%d\n", lp[n]);
		}
	}

}
