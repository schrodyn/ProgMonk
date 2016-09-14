#include <cstdio>

typedef long long ll;

ll gcd(ll a, ll b)
{
	if(b == 0)
		return a;
	return gcd(b, a % b);
}

// Finds such x and y, that a * x + b * y = gcd(a, b)
ll extended_gcd(ll a, ll b, ll &x, ll &y)
{
	if(b == 0) {
		x = 1; y = 0;
		return a;
	}
	ll x1, y1;
	ll g = extended_gcd(b, a%b, x1, y1);

	x = y1;
	y = x1 - (a/b)*y1;

	return g;
}

// Solves equation a * x + b * y = c, writes answer to x and y
int solve_diophant(ll a, ll b, ll c, ll &x, ll &y)
{
	ll g = extended_gcd(a, b, x, y);

	if(c % g != 0) {
		printf("Impossible\n");
		return -1;
	}

	c /= g;
	x *= c; y *= c;

	return 0;
}

int main()
{
	ll a, b, c, x, y;
	int err;
	scanf("%lld %lld %lld", &a, &b, &c);

	err = solve_diophant(a, b, c, x, y);
	if(!err)
		printf("x = %lld; y = %lld\n", x, y);
}


