#include <cstdio>
#include <vector>

using namespace std;

vector<bool> prime;

void sieve(int n)
{
	prime.assign(n+1,true);
	prime[0] = prime[1] = false;
	for(int i = 3; i * 1ll * i <= n; i+=2)
		if(prime[i])
			for(int j = i * i; j <= n; j+=i)
				prime[j] = false;
}

bool check(int n)
{
	if(n % 2 == 0)
		return false;
	return prime[n];
}

int main()
{
	int n,t;
	scanf("%d %d", &n, &t);
	sieve(n);
	while(t--) {
		scanf("%d", &n);
		printf("%s\n", check(n) ? "YES" : "NO");
	}
}

