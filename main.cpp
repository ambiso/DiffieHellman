#include <iostream>
#include <random>
#include <gmp.h>
#include <unordered_set>
#include <inttypes.h>
#include <assert.h>

using namespace std;

typedef unordered_set<const mpz_t*> mpztuset;

bool isPrime(int64_t x);
bool isPrimitiveRoot(const mpz_t a, const mpz_t m);
void totient(mpz_t rop, const mpz_t x);
mpztuset factorize(const mpz_t x);

int main(int argc, char **argv)
{
    mpz_t p, g, a, B, facsize, sec, i, j, tmp, tmp2;
    mpz_inits(p, g, a, B, facsize, sec, i, j, tmp, tmp2, NULL);
    string buf;
    mt19937 gen;
    mpztuset factors;
    mpz_set_ui(sec, 1);
    if(argc > 1)
    {
    	mpz_set_str(sec, argv[1], 10);
    }
    cout << "[M]anual or [A]uto?" << endl;
    getline(cin, buf);
    switch(buf[0]) {
        case 'm':
        case 'M':
            cout << "Enter prime: \t";
            gmp_scanf("%Zd", p);

            if(mpz_probab_prime_p(p, 25) == 0)
            {
				gmp_printf("%Zd is not a prime.", p);
                return 1;
            }
            cout << "Enter base: \t";
            gmp_scanf("%Zd", g);
            if(mpz_cmp_si(g, 1) <= 0) //g <= 1
            {
                cout << "Base must be higher than 1" << endl;
                return 1;
            }
            /*if(!isPrimitiveRoot(g, p))
            {
				gmp_printf("%Zd is not a primitive root of %Zd\n", g, p);
                return 1;
            }*/
            cout << "Enter secret: \t";
            gmp_scanf("%Zd", a);
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
            gmp_printf("%Zd\n", p);
            cout << "Gen'd base: \t";
            mpz_sub_ui(tmp, p, 1);
            factors = factorize(tmp);
            mpz_set_ui(facsize, factors.size());
            for(mpz_set_ui(i, 2); mpz_cmp(i, p) < 0; mpz_add_ui(i, i, 1))
            {
				mpz_set_ui(j, 0);
                for(auto it = begin(factors); it != end(factors); it++)
                {
					mpz_sub_ui(tmp, p, 1);
					mpz_tdiv_q(tmp, tmp, *(*it));
					mpz_powm(tmp, i, tmp, p);
                    if(mpz_cmp_ui(tmp, 1) == 0) //modpow(i, (p-1) / *it, p) == 1
                        break;
                    mpz_add_ui(j, j, 1);
                }
                if(mpz_cmp(j, facsize) == 0)
                {
                    mpz_set(g, i);
                    break;
                }
            }
            if(mpz_cmp_ui(g, 0) == 0)
            {
                cout << "This should never happen. Please report this issue" << endl;
                return 1;
            }
            gmp_printf("%Zd\n", g);
            mpz_sub_ui(tmp, p, 2); //p-2
            mpz_set_ui(tmp2, gen()); //gen()
            mpz_tdiv_r(tmp, tmp2, tmp); //gen() % (p-2)
            mpz_add_ui(tmp, tmp, 2); // (gen() % (p-2)) + 2
            mpz_set(a, tmp); // a = (gen() % (p-2)) + 2
            gmp_printf("Gen'd secret: \t%Zd\n", a);
            break;
        default:
            cout << "Error, wrong input." << endl;
            return 1;
    }
    mpz_sub_ui(tmp, p, 1);
    if(mpz_cmp_ui(a, 2) < 0 || mpz_cmp(a, tmp) > 0)
    {
		gmp_printf("Please choose a secret between 1 and %Zd\n", p);
        return 1;
    }
    mpz_powm(tmp, g, a, p); //modpow(g, a, p)
    gmp_printf("Send Bob:\t%Zd\n", tmp);
    cout << "Enter B: \t";
    gmp_scanf("%Zd", B);
    mpz_powm(tmp, B, a, p); //tmp
    gmp_printf("Shared secret: \t%Zd\n", tmp);
    return 0;
}

void totient(mpz_t rop, const mpz_t x)
{
    assert(mpz_cmp_si(x, 0) >= 0);
    mpz_t i, tmp;
    mpz_set_si(rop, 1);
    for(mpz_inits(i, tmp, NULL); mpz_cmp(i, x) < 0; mpz_add_ui(i, i, 1))
    {
		mpz_gcd(tmp, x, i);
        if(mpz_cmp_ui(tmp, 1) == 0) //gcd(x, i) == 1
            mpz_add_ui(rop, rop, 1);
    }
    mpz_clear(i);
    mpz_clear(tmp);
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
				mpz_t* to_insert = reinterpret_cast<mpz_t*>(malloc(sizeof(mpz_t)));
				mpz_init(*to_insert);
				mpz_set(*to_insert, i);
				factors.insert(to_insert);
			}
			mpz_tdiv_q(xs, xs, i);
			mpz_mod(tmp, xs, i);
			k++;
        }
    }
    return factors;
}

bool isPrimitiveRoot(const mpz_t a, const mpz_t p) //DEPRECATED
{
    assert(mpz_cmp_si(a, 0) > 0 && mpz_cmp_si(p, 0)  > 0);
    if(mpz_cmp(a, p) > 0)
        return false;

    mpz_t s, x, i, tmp;
    mpz_inits(s, x, i, tmp, NULL);
    totient(s, p);
    mpz_set(x, s);
    mpz_set_ui(i, 2);
    mpz_tdiv_q(tmp, x, i);
    for(mpz_set_si(i, 2); mpz_cmp(i,  tmp) <= 0; mpz_add_ui(i, i, 1))
    {
		mpz_tdiv_r(tmp, x, i);
        while(mpz_cmp_ui(tmp, 0) == 0)
        {
			mpz_tdiv_q(tmp, s, i);
			mpz_powm(tmp, a, tmp, p);
            if(mpz_cmp_ui(tmp, 1) == 0)
                return false;
            mpz_tdiv_q(x, x, i);
        }
		mpz_tdiv_q(tmp, x, i);
    }
    if(mpz_cmp_ui(x, 1) > 0)
    {
		mpz_tdiv_q(tmp, s, x);
		mpz_powm(tmp, a, tmp, p);
        if(mpz_cmp_ui(tmp, 1) == 0) //modpow(a, s / x, p) == 1
            return false;
    }
    return true;
}
