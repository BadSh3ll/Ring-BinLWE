#ifndef UTILS_H
#define UTILS_H

#include "poly.h"

unsigned short reduce(unsigned short x);
void print(unsigned short *x, int len);

void encode_m(poly *mhat, const unsigned short *msg);
void decode_m(unsigned short *msg, poly *mhat);


void encode_p(unsigned short *pk, const poly *a, const poly *p);
void decode_p(poly *a, poly *p, const unsigned short *pk);


void encode_c(const poly *c1, const poly *c2, unsigned short *ct);
void decode_c(poly *c1, poly *c2, const unsigned short *ct);

#endif // UTILS_H