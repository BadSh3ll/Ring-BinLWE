#include "params.h"
#include "poly.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>

poly poly_mul(poly a, poly b) {
    poly result;

    memset(result.coeffs, 0, sizeof(unsigned short) * PARAMS_N);
    for (int i = 0; i < PARAMS_N; i++) {
        if (b.coeffs[i] == 0) continue;
        for (int j = 0; j < PARAMS_N; j++) {
            int k = (i + j) % PARAMS_N;
            int sign = ((i + j) >= PARAMS_N) ? -1 : 1;
            result.coeffs[k] = reduce(result.coeffs[k] + sign * a.coeffs[j]);
        }
    }

    return result;
}

poly poly_add(poly a, poly b) {
    poly result;
    for (int i = 0; i < PARAMS_N; i++) {
        result.coeffs[i] = reduce(a.coeffs[i] + b.coeffs[i]);
    }
    return result;
}

poly poly_sub(poly a, poly b) {
    poly result;
    for (int i = 0; i < PARAMS_N; i++) {
        result.coeffs[i] = reduce(a.coeffs[i] - b.coeffs[i]);
    }
    return result;
}

poly poly_frommsg(const unsigned short *msg) {
    poly result;
    for (int i = 0; i < PARAMS_N; i++) {
        result.coeffs[i] = msg[i];
    }
    return result;
}