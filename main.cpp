#include <iostream>
#include <random>
#include <unordered_set>
#include <inttypes.h>
#include <assert.h>

using namespace std;

bool isPrime(int64_t x);
int64_t pow(int64_t b, int64_t e);
bool isPrimitiveRoot(int64_t pr, int64_t m);
int64_t totient(int64_t x);
int64_t gcd(int64_t a, int64_t b);

int main()
{
    int64_t p, g, a, B;
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
            cout << "Send Bob: \t" << pow(g, a) % p << endl;
            cout << "Enter B: \t";
            cin >> B;
            cout << "Shared secret: \t" << pow(B,a) % p;
            break;
        case 'a':
        case 'A':

            break;
        default:
            cout << "Error, wrong input." << endl;
            return 1;
    }
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

int64_t pow(int64_t b, int64_t e)
{
    assert(e >= 0);
    int64_t res = 1;
    for(int64_t i = 0; i < e; i++)
        res *= b;
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

bool isPrimitiveRoot(int64_t a, int64_t p)
{
    assert(a > 0 && p > 0);
    if(a > p)
        return false;

    int64_t s = totient(p), x = s;

    for(int64_t i = 2; i <= x; i++)
    {
        while(x % i == 0)
        {
            if(pow(a, s / i) % p == 1)
                return false;
            x /= i;
        }
    }
    return true;
}
