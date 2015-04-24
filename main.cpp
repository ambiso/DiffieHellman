#include <iostream>
#include <gmpxx.h>
#include <random>
#include <unordered_set>
#include <inttypes.h>
#include <assert.h>

using namespace std;

typedef unordered_set<const mpz_t*> mpztuset;

bool isPrime(int64_t x);
int64_t spow(int64_t b, int64_t e);
int64_t modpow(int64_t b, int64_t e, int64_t m);
bool isPrimitiveRoot(const mpz_t a, const mpz_t m);
void totient(mpz_t rop, const mpz_t x);
mpztuset factorize(mpz_t x);

int main(int argc, char **argv)
{
    mpz_t p, g, a, B, facsize, sec;
    mpz_inits(p, g, a, B, facsize, sec, NULL);
    string buf;
    mt19937 gen;
    mpztuset factors;
    
    if(argc > 1)
    {
    	mpz_set_str(sec, argv[1], 10);
    }
    cout << "[M]anual or [A]uto?" << endl;
    char c;
    cin >> c;
    switch(c) {
        case 'm':
        case 'M':
            cout << "Enter prime: \t";
            getline(cin, buf);
	    mpz_set_str(p, buf.c_str(), 10);
            if(mpz_probab_prime_p(p, 25) != 2)
            {
                cout << p << " is (probably) not a prime. Continue? y/n" << endl;
                cin >> c;
		if(c != 'y' && c != 'Y') 
		{
		    return 1;
		}
            }
            cout << "Enter base: \t";
            cin >> g;
            if(mpz_cmp_si(g, 1) <= 0) //g <= 1
            {
                cout << "Base must be higher than 1" << endl;
                return 1;
            }
            if(!isPrimitiveRoot(g, p))
            {
                cout << g << " is not a primitive root of " << p << endl;
                return 1;
            }
            cout << "Enter secret: \t";
            cin >> a;
            break;
        case 'a':
        case 'A':
            cout << "Enter random numbers: ";
            unsigned int seed;
            cin >> seed;
            gen.seed(seed);
            cout << "Gen'd prime: \t";
            mpz_set_ui(p, gen());
            mpz_tdiv_q(p, p, sec);
            mpz_add_ui(p, p, 2);
            mpz_nextprime(p, p);
            cout << p << endl;
            cout << "Gen'd base: \t";
            factors = factorize(p-1);
            facsize = factors.size();
            for(int64_t i = 2; i < p; i++)
            {
                int64_t j = 0;
                for(auto it = begin(factors); it != end(factors); it++)
                {
                    if(modpow(i, (p-1) / *it, p) == 1)
                        break;
                    j++;
                }
                if(j == facsize)
                {
                    g = i;
                    break;
                }
            }
            if(g == 0)
            {
                cout << "This should never happen. Please report this issue" << endl;
                return 1;
            }
            cout << g << endl;
            a = gen() % (p-2) + 2;
            cout << "Gen'd secret: \t" << a << endl;
            break;
        default:
            cout << "Error, wrong input." << endl;
            return 1;
    }
    if(a < 2 || a > p-1)
    {
        cout << "Please choose a secret between 1 and " << p << endl;
        return 1;
    }
    cout << "Send Bob: \t" << modpow(g, a, p) << endl;
    cout << "Enter B: \t";
    cin >> B;
    cout << "Shared secret: \t" << modpow(B, a, p) << endl;
    return 0;
}

mpz_t totient(mpz_t rop, const mpz_t x)
{
    assert(mpz_cmpsi(x, 0) >= 0);
    mpz_t i, tmp;
    mpz_set_si(rop, rop, 1);
    mpz_inits(i, tmp, NULL);
    for(; mpz_cmp(i, x) < 0; mpz_add_ui(i, i, 1))
    {
	mpz_gcd(tmp, x, i);
        if(mpz_cmp(tmp, 1) == 0) //gcd(x, i) == 1
            mpz_add_ui(res, res, 1);
    }
    mpz_clear(i);
    mpz_clear(tmp);
    return res;
}


mpztuset factorize(const mpz_t x)
{
    mpztuset factors;
    mpz_t i, xs, tmp;
    mpz_inits(i, tmp, xs, NULL);
    mpz_set(xs, x);
    for(mpz_set_si(i, 2); mpz_cmp(i, xs) <= 0; mpz_add_ui(i, i, 1))
    {
		mpz_mod(tmp, xs, i);
		int k = 0;
        while(mpz_cmp_ui(tmp, 0) == 0)
        {
			if(k == 0)
			{
				mpz_t to_insert;
				mpz_init(to_insert);
				mpz_set(to_insert, i);
				factors.insert(to_insert);
			}
			mpz_div_q(x, x, i);
			k++;
        }
    }
    return factors;
}

bool isPrimitiveRoot(const mpz_t a, const mpz_t p)
{
    assert(mpz_cmpsi(a, 0) > 0 && mpz_cmpsi(p, 0)  > 0);
    if(mpz_cmp(a, p) > 0)
        return false;

    mpz_t s, x, i, tmp;
    mpz_inits(s, x, i, tmp, NULL);
    mpz_set(s, totient(p));
    mpz_set(x, s);
    mpz_
    for(mpz_set_si(i, 2); mpz_cmp(i,  x/i; i++)
    {
        while(x % i == 0)
        {
            if(modpow(a, s / i, p) == 1)
                return false;
            x /= i;
        }
    }
    if(x > 1)
        if(modpow(a, s / x, p) == 1)
            return false;
    return true;
}
