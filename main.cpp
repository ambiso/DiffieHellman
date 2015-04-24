#include <iostream>
#include <random>
#include <unordered_set>
#include <inttypes.h>
#include <assert.h>

using namespace std;

typedef unordered_set<int64_t> i64uset;

bool isPrime(int64_t x);
int64_t spow(int64_t b, int64_t e);
int64_t modpow(int64_t b, int64_t e, int64_t m);
bool isPrimitiveRoot(int64_t pr, int64_t m);
int64_t totient(int64_t x);
int64_t gcd(int64_t a, int64_t b);
i64uset factorize(int64_t x);

int main()
{
    int64_t p = 0, g = 0, a = 0, B = 0, facsize;
    mt19937 gen;
    i64uset factors;
    cout << "[M]anual or [A]uto?" << endl;
    char c;
    cin >> c;
    switch(c) {
        case 'm':
        case 'M':
            cout << "Enter prime: \t";
            cin >> p;
            if(!isPrime(p))
            {
                cout << p << " is not a prime." << endl;
                return 1;
            }
            cout << "Enter base: \t";
            cin >> g;
            if(g <= 1)
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
            while(p == 0)
                p = gen()/100000;
            while(!isPrime(p))
                p++;
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
            a = gen() % p;
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

bool isPrime(int64_t x)
{
    for(int64_t i = 2; i*i <= x; i++)
    {
        if(x % i == 0)
            return false;
    }
    return true;
}

int64_t spow(int64_t b, int64_t e)
{
    assert(e >= 0);
    int64_t res = 1;
    for(int64_t i = 0; i < e; i++)
        res *= b;
    return res;
}

int64_t modpow(int64_t b, int64_t e, int64_t m)
{
    assert(e >= 0 && m > 1);
    int64_t res = 1;
    for(int64_t i = 0; i < e; i++) {
        res *= b;
        res %= m;
    }
    return res;
}

int64_t totient(int64_t x)
{
    assert(x >= 0);
    int64_t res = 0;
    for(int64_t i = 1; i < x; i++)
    {
        if(gcd(x, i) == 1)
            res++;
    }
    return res;
}

int64_t gcd(int64_t a, int64_t b)
{
    assert(a > 0 && b > 0);
    while(b > 0)
    {
        int64_t h = a % b;
        a = b;
        b = h;
    }
    return a;
}

i64uset factorize(int64_t x)
{
    i64uset factors;
    for(int64_t i = 2; i <= x; i++)
    {
        while(x % i == 0)
        {
            factors.insert(i);
            x /= i;
        }
    }
    return factors;
}

bool isPrimitiveRoot(int64_t a, int64_t p)
{
    assert(a > 0 && p > 0);
    if(a > p)
        return false;

    int64_t s = totient(p), x = s;

    for(int64_t i = 2; i <= x/i; i++)
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
