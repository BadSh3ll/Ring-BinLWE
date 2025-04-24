#include <stdio.h>
#include <stdlib.h>

#include "params.h"
#include "poly.h"

void print(unsigned short *x, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d ", x[i]);
    }
    printf("\n");
}
 
// barrett reduction
unsigned short reduce(short x) {
    // Reduce x modulo Q
    x = x % PARAMS_Q;
    if (x < 0) {
        x += PARAMS_Q;
    }
    return (unsigned short)x;
}

// Encode the message
void encode_m(poly *mhat, const unsigned short *msg) {
    for (int i = 0; i < PARAMS_N; i++) {
        mhat->coeffs[i] = msg[i] ? HALF_Q : 0;
    }
}

void decode_m(unsigned short *msg, poly *mhat) {
    for (int i = 0; i < PARAMS_N; i++) {
        // Check if the coefficient is closer to HALF_Q
        if (abs((short)mhat->coeffs[i] - HALF_Q) <= Q4) {
            msg[i] = 1; // Close to HALF_Q
        } else {
            msg[i] = 0; // Far from HALF_Q
        }
    }
}

// Encode the public key
// pk = a || p
void encode_p(unsigned short *pk, const poly *a, const poly *p) {
    for (int i = 0; i < PARAMS_N; i++) {
        pk[i] = a->coeffs[i];
        pk[i+PARAMS_N] = p->coeffs[i];
    }
}
void decode_p(poly *a, poly *p, const unsigned short *pk) {
    for (int i = 0; i < PARAMS_N; i++) {
        a->coeffs[i] = pk[i];
        p->coeffs[i] = pk[i+PARAMS_N];
    }
}
// Encode the ciphertext
void encode_c(const poly *c1, const poly *c2, unsigned short *ct) {
    for (int i = 0; i < PARAMS_N; i++) {
        ct[i] = c1->coeffs[i];
        ct[i+PARAMS_N] = c2->coeffs[i];
    }
}

void decode_c(poly *c1, poly *c2, const unsigned short *ct) {
    for (int i = 0; i < PARAMS_N; i++) {
        c1->coeffs[i] = ct[i];
        c2->coeffs[i] = ct[i+PARAMS_N];
    }
}

