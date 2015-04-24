#pragma once

#ifndef HELPERS_HPP_INCLUDED
#define HELPERS_HPP_INCLUDED

#include <iostream>
#include <gmp.h>
#include <unordered_set>
#include <assert.h>

typedef std::unordered_set<const mpz_t*> mpztuset;

bool isPrime(int64_t x);
bool isPrimitiveRoot(const mpz_t a, const mpz_t m);
void gen_base(mpz_t rop, mpz_t p);
void totient(mpz_t rop, const mpz_t x);
mpztuset factorize(const mpz_t x);

#endif // HELPERS_HPP_INCLUDED
