#ifndef POLY_H
#define POLY_H

#include "params.h"

typedef struct {
    unsigned short coeffs[PARAMS_N];
} poly;


poly poly_mul(poly a, poly b);
poly poly_add(poly a, poly b);
poly poly_sub(poly a, poly b);
poly poly_frommsg(const unsigned short *msg);

#endif // POLY_H