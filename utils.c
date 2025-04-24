#include <stdio.h>
#include <stdlib.h>

#include "params.h"
#include "poly.h"

void print(unsigned char *x, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d ", x[i]);
    }
    printf("\n");
}

// Encode the message
void encode_m(poly *mhat, const unsigned char *msg) {
    for (int i = 0; i < PARAMS_N; i++) {
        mhat->coeffs[i] = msg[i] ? HALF_Q : 0;
    }
}

void decode_m(unsigned char *msg, poly *mhat) {
    for (int k = 0; k < PARAMS_N; k++) {
        int diff = abs(mhat->coeffs[k] - HALF_Q);
        msg[k] = (diff <= Q4);
    }
}

// Encode the public key
// pk = a.p
void encode_p(unsigned char *pk, const poly *a, const poly *p) {
    for (int i = 0; i < PARAMS_N; i++) {
        pk[i] = a->coeffs[i];
        pk[i+PARAMS_N] = p->coeffs[i];
    }
}
void decode_p(poly *a, poly *p, const unsigned char *pk) {
    for (int i = 0; i < PARAMS_N; i++) {
        a->coeffs[i] = pk[i];
        p->coeffs[i] = pk[i+PARAMS_N];
    }
}
// Encode the ciphertext
void encode_c(const poly *c1, const poly *c2, unsigned char *ct) {
    for (int i = 0; i < PARAMS_N; i++) {
        ct[i] = c1->coeffs[i];
        ct[i+PARAMS_N] = c2->coeffs[i];
    }
}

void decode_c(poly *c1, poly *c2, const unsigned char *ct) {
    for (int i = 0; i < PARAMS_N; i++) {
        c1->coeffs[i] = ct[i];
        c2->coeffs[i] = ct[i+PARAMS_N];
    }
}

