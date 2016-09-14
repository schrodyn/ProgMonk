#include <cstdio>
#include <vector>
#include <bitset>

using namespace std;

typedef long long ll;

const int NMAX = (int)1e7 + 5;

bitset<NMAX+1> bs;
vector<int> primes;

void sieve(ll n)
{
	bs.set();	//set all bits to 1
	bs[0] = bs[1] = 0; //except index 0 and 1
	for(ll i = 3; i * i <= n; i+=2)
		if(bs[i]) {
			for(ll j = i * i; j <= n; j+=i)
				bs[j] = 0;
			primes.push_back((int)i);
		}
}

// note: only work for n <= (last prime in vi "primes")^2
bool check(ll n, ll sieve_size)
{
	if(n % 2 == 0)
		return false;
	
	if(n <= sieve_size)
		return bs[n];
	
	for(int i = 0; i < (int)primes.size(); i++)
		if(n % primes[i] == 0) 
			return false;

	return true;
}

int main()
{
	sieve((ll)NMAX); // can go up to 10^7 (need few seconds))
	printf("%d\n", check(1000003, (ll)NMAX)); //is prime
	printf("%d\n", check(1000007, (ll)NMAX)); //not prime
}


