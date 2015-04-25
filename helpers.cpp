#include "helpers.hpp"

void gen_base(mpz_t rop, mpz_t p)
{
    mpz_t tmp, i, facsize, j, g;
    mpz_inits(tmp, i, j, facsize, g, NULL);
    mpz_sub_ui(tmp, p, 1);
    mpztuset factors = factorize(tmp);
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
        std::cerr << "This should never happen. Please report this issue" << std::endl;
        exit(1);
    }
    mpz_set(rop, g);
    mpz_clears(tmp, i, facsize, j, g, NULL);
}

void phi(mpz_t rop, const mpz_t x)
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
    mpz_clears(tmp, i, NULL);
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
    mpz_clears(i, xs, tmp, NULL);
    return factors;
}

bool isPrimitiveRoot(const mpz_t a, const mpz_t p) //DEPRECATED
{
    assert(mpz_cmp_si(a, 0) > 0 && mpz_cmp_si(p, 0)  > 0);
    if(mpz_cmp(a, p) > 0)
        return false;

    mpz_t s, x, i, tmp;
    mpz_inits(s, x, i, tmp, NULL);
    phi(s, p);
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
