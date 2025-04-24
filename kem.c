#include <stdio.h>
#include <string.h>

#include "kem.h"
#include "random.h"
#include "params.h"
#include "utils.h"
#include "poly.h"


void keygen(unsigned char *pk, unsigned char *sk) {

    // random polynomial a
    poly a;
    randombytes(a.coeffs, PARAMS_N);
    // random polynomials r1 and r2 with binary coefficients
    poly r1;
    poly r2;
    sampleBinary(r1.coeffs, PARAMS_N);
    sampleBinary(r2.coeffs, PARAMS_N);
    // Compute the public key p = r1 - a * r2
    poly p = poly_sub(r1, poly_mul(a, r2));
    encode_p(pk, &a, &p);
    memcpy(sk, r2.coeffs, PARAMS_N);
}

void enc(unsigned char *ct, const unsigned char *m, const unsigned char *pk) {
    
    // Encode the message
    poly mhat;
    encode_m(&mhat, m);
    // Decode the public key
    poly a, p;
    decode_p(&a, &p, pk);

    // Generate a random polynomials e1, e2, e3 with binary coefficients
    poly e1, e2, e3;
    sampleBinary(e1.coeffs, PARAMS_N);
    sampleBinary(e2.coeffs, PARAMS_N);
    sampleBinary(e3.coeffs, PARAMS_N);

    // Compute the ciphertext components
    // c1 = a * e1 + e2
    // c2 = p * e1 + e3 + mhat
    poly c1 = poly_add(poly_mul(a, e1), e2);
    poly c2 = poly_add(poly_mul(p, e1), poly_add(e3, mhat));

    // Encode the ciphertext
    // ct = c1 || c2
    encode_c(&c1, &c2, ct);    
}

void dec(unsigned char *m, const unsigned char *ct, const unsigned char *sk) {
    
    // Decode the ciphertext
    poly c1, c2;
    decode_c(&c1, &c2, ct);

    poly r2 = poly_frommsg(sk);
    // Compute the encoded message
    poly mhat = poly_add(poly_mul(c1, r2), c2);
    decode_m(m, &mhat);
}