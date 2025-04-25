#include <stdio.h>
#include <string.h>

#include "kem.h"
#include "random.h"
#include "params.h"
#include "utils.h"
#include "poly.h"


/**
 * @brief Generates a public and secret key pair for the cryptographic scheme.
 *
 * This function generates a public key (pk) and a secret key (sk) based on
 * the Ring Learning with Errors (R-LWE) cryptographic scheme. The public key
 * consists of a uniformly random polynomial `a` and a polynomial `p` derived
 * from `a`, `r1`, and `r2`. The secret key is the polynomial `r2` with binary
 * coefficients.
 *
 * @param[out] pk Pointer to the array where the public key will be stored.
 *                The public key is encoded as `a || p`.
 * @param[out] sk Pointer to the array where the secret key will be stored.
 *                The secret key is the binary polynomial `r2`.
 *
 * @details
 * - A uniformly random polynomial `a` is generated.
 * - Two binary polynomials `r1` and `r2` are sampled uniformly at random.
 * - The polynomial `p` is computed as `p = r1 - a * r2`.
 * - The public key is encoded as the concatenation of `a` and `p`.
 * - The secret key is set to the coefficients of `r2`.
 *
 * @note The function assumes that the arrays `pk` and `sk` have been
 *       pre-allocated with sufficient space to store the keys.
 */
void keygen(unsigned short *pk, unsigned short *sk) {

    // Generate a uniformly random polynomial a
    // a = a_0 + a_1 * x + ... + a_{N-1} * x^{N-1}
    poly a;
    GenA(a.coeffs, PARAMS_N);
    
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


/**
 * @brief Encrypts a message using the public key and generates a ciphertext.
 *
 * This function performs encryption in a lattice-based cryptographic scheme.
 * It encodes the message into polynomial form, decodes the public key into
 * its polynomial components, generates random binary noise polynomials, and
 * computes the ciphertext as two concatenated polynomials.
 *
 * @param[out] ct  Pointer to the output ciphertext. The ciphertext is encoded
 *                 as a concatenation of two polynomials.
 * @param[in]  m   Pointer to the input message. The message is represented
 *                 as a polynomial with coefficients in a finite field.
 * @param[in]  pk  Pointer to the public key. The public key is encoded and
 *                 needs to be decoded into its polynomial components.
 *
 * @details
 * The encryption process involves the following steps:
 * 1. Encode the input message `m` into a polynomial `mhat`.
 * 2. Decode the public key `pk` into polynomials `a` and `p`.
 * 3. Generate three random binary noise polynomials `e1`, `e2`, and `e3`.
 * 4. Compute the first part of the ciphertext `c1` as:
 *    c1 = a * e1 + e2
 * 5. Compute the second part of the ciphertext `c2` as:
 *    c2 = p * e1 + e3 + mhat
 * 6. Encode the ciphertext as the concatenation of `c1` and `c2`.
 *
 * The function assumes that the input parameters are properly initialized
 * and that the output buffer `ct` has sufficient space to store the ciphertext.
 */

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

/**
 * @brief Decrypts a ciphertext to recover the original message.
 *
 * This function performs decryption by decoding the ciphertext into two polynomials,
 * computing an intermediate polynomial using the secret key, and then recovering
 * the original message.
 *
 * @param[out] m   Pointer to the output buffer where the decrypted message will be stored.
 * @param[in]  ct  Pointer to the ciphertext to be decrypted.
 * @param[in]  sk  Pointer to the secret key used for decryption.
 *
 * The decryption process involves:
 * 1. Decoding the ciphertext into two polynomials `c1` and `c2`.
 * 2. Converting the secret key into a polynomial `r2`.
 * 3. Computing an intermediate polynomial `mhat` as `mhat = c1 * r2 + c2`.
 * 4. Decoding the message from the intermediate polynomial `mhat`.
 */
void dec(unsigned short *m, const unsigned short *ct, const unsigned short *sk) {
    
    poly c1, c2;
    decode_c(&c1, &c2, ct);
    // Decode the ciphertext
    poly r2 = poly_frommsg(sk);
    // Compute mhat = c1*r2 + c2
    poly mhat = poly_add(poly_mul(c1, r2), c2);
    decode_m(m, &mhat);
}