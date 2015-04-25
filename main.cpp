#include <iostream>
#include <random>
#include <gmp.h>
#include <assert.h>
#include "helpers.hpp"

int main(int argc, char **argv)
{
    mpz_t p, g, a, B, facsize, sec, i, j, tmp, tmp2;
    mpz_inits(p, g, a, B, facsize, sec, i, j, tmp, tmp2, NULL);
    std::string buf;
    std::mt19937 gen;
    mpz_set_ui(sec, 1);
    if(argc > 1)
    {
    	mpz_set_str(sec, argv[1], 10);
    }
    std::cout << "[M]anual or [A]uto?" << std::endl;
    getline(std::cin, buf);
    switch(buf[0]) {
        case 'm':
        case 'M':
            std::cout << "Enter prime: \t";
            gmp_scanf("%Zd", p);

            if(mpz_probab_prime_p(p, 25) == 0)
            {
				gmp_printf("%Zd is not a prime.\n", p);
				mpz_nextprime(p, p);
				gmp_printf("Using %Zd instead.\n", p);
            }
            std::cout << "Enter base: \t";
            gmp_scanf("%Zd", g);
            if(mpz_cmp_si(g, 1) <= 0) //g <= 1
            {
                std::cerr << "Base is lower than 1." << std::endl;
                gen_base(g, p);
                gmp_printf("Using %Zd as base.\n", g);
            }
            /*if(!isPrimitiveRoot(g, p))
            {
				gmp_printf("%Zd is not a primitive root of %Zd\n", g, p);
                return 1;
            }*/
            std::cout << "Enter secret: \t";
            gmp_scanf("%Zd", a);
            break;
        case 'a':
        case 'A':
            std::cout << "Enter random numbers: ";
            unsigned int seed;
            std::cin >> seed;
            gen.seed(seed);
            std::cout << "Gen'd prime: \t";
            mpz_set_ui(p, gen());
            mpz_tdiv_q(p, p, sec);
            mpz_add_ui(p, p, 2);
            mpz_nextprime(p, p);
            gmp_printf("%Zd\n", p);
            std::cout << "Gen'd base: \t";
            gen_base(g, p);
            gmp_printf("%Zd\n", g);
            mpz_sub_ui(tmp, p, 2); //p-2
            mpz_set_ui(tmp2, gen()); //gen()
            mpz_tdiv_r(tmp, tmp2, tmp); //gen() % (p-2)
            mpz_add_ui(tmp, tmp, 2); // (gen() % (p-2)) + 2
            mpz_set(a, tmp); // a = (gen() % (p-2)) + 2
            gmp_printf("Gen'd secret: \t%Zd\n", a);
            break;
        default:
            std::cout << "Error, wrong input." << std::endl;
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
    std::cout << "Enter B: \t";
    gmp_scanf("%Zd", B);
    mpz_powm(tmp, B, a, p); //tmp
    gmp_printf("Shared secret: \t%Zd\n", tmp);
    return 0;
}
