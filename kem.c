#include <stdio.h>
#include <string.h>

#include "kem.h"
#include "random.h"
#include "params.h"
#include "utils.h"
#include "poly.h"


void keygen(unsigned short *pk, unsigned short *sk) {

    // Generate a uniformly random polynomial a
    // a = a_0 + a_1 * x + ... + a_{N-1} * x^{N-1}
    poly a;
    randombytes(a.coeffs, PARAMS_N);
    
    // Generate uniformly random polynomials r1, r2 with binary coefficients
    poly r1, r2;
    sampleBinary(r1.coeffs, PARAMS_N);
    sampleBinary(r2.coeffs, PARAMS_N);
    
    // Compute p = r1 - a * r2
    poly p = poly_sub(r1, poly_mul(a, r2));

    // Public key pk = a || p
    encode_p(pk, &a, &p);

    // Secret key sk = r2
    for (int i = 0; i < PARAMS_N; i++) {
        sk[i] = r2.coeffs[i];
    }
}

void enc(unsigned short *ct, const unsigned short *m, const unsigned short *pk) {
    
    poly mhat;
    encode_m(&mhat, m);

    poly a, p;
    decode_p(&a, &p, pk);
    
    // Generate noises e1, e2, e3 with binary coefficients
    poly e1, e2, e3;
    sampleBinary(e1.coeffs, PARAMS_N);
    sampleBinary(e2.coeffs, PARAMS_N);
    sampleBinary(e3.coeffs, PARAMS_N);

    // Compute c1 = a * e1 + e2
    // Compute c2 = p * e1 + e3 + mhat
    poly c1 = poly_add(poly_mul(a, e1), e2);
    poly c2 = poly_add(poly_mul(p, e1), poly_add(e3, mhat));

    // Ciphertext ct = c1 || c2
    encode_c(&c1, &c2, ct);
}

void dec(unsigned short *m, const unsigned short *ct, const unsigned short *sk) {
    
    poly c1, c2;
    decode_c(&c1, &c2, ct);
    // Decode the ciphertext
    poly r2 = poly_frommsg(sk);
    // Compute mhat = c1*r2 + c2
    poly mhat = poly_add(poly_mul(c1, r2), c2);
    decode_m(m, &mhat);
}