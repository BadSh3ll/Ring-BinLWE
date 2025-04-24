#ifndef UTILS_H
#define UTILS_H

#include "poly.h"

int barrett_reduce(unsigned char x);
void print(unsigned char *x, int len);

void encode_m(poly *mhat, const unsigned char *msg);
void decode_m(unsigned char *msg, poly *mhat);


void encode_p(unsigned char *pk, const poly *a, const poly *p);
void decode_p(poly *a, poly *p, const unsigned char *pk);


void encode_c(const poly *c1, const poly *c2, unsigned char *ct);
void decode_c(poly *c1, poly *c2, const unsigned char *ct);

#endif // UTILS_H