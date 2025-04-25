#include <string.h>

#include "params.h"
#include "poly.h"
#include "utils.h"

/**
 * @brief Multiplies two polynomials in a ring and returns the result.
 *
 * This function performs polynomial multiplication in a ring of size PARAMS_N.
 * The coefficients of the result are reduced using the `reduce` function.
 * If the sum of indices exceeds PARAMS_N, the sign of the coefficient is adjusted.
 *
 * @param a The first polynomial operand.
 * @param b The second polynomial operand.
 * @return The resulting polynomial after multiplication.
 *
 * @note The function assumes that the input polynomials `a` and `b` have their
 *       coefficients stored in arrays of size PARAMS_N.
 */
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

/**
 * @brief Adds two polynomials and reduces the coefficients.
 *
 * This function takes two input polynomials `a` and `b`, adds their
 * coefficients element-wise, and reduces the result using the `reduce`
 * function. The resulting polynomial is returned.
 *
 * @param a The first input polynomial.
 * @param b The second input polynomial.
 * @return A polynomial representing the sum of `a` and `b` with reduced coefficients.
 */
poly poly_add(poly a, poly b) {
    poly result;
    for (int i = 0; i < PARAMS_N; i++) {
        result.coeffs[i] = reduce(a.coeffs[i] + b.coeffs[i]);
    }
    return result;
}

/**
 * @brief Subtracts two polynomials element-wise and reduces the result.
 *
 * This function computes the difference of two polynomials `a` and `b` 
 * by subtracting their coefficients element-wise. The result is reduced 
 * using the `reduce` function to ensure coefficients remain within the 
 * desired range.
 *
 * @param a The first polynomial (minuend).
 * @param b The second polynomial (subtrahend).
 * @return A polynomial representing the result of the subtraction.
 */
poly poly_sub(poly a, poly b) {
    poly result;
    for (int i = 0; i < PARAMS_N; i++) {
        result.coeffs[i] = reduce(a.coeffs[i] - b.coeffs[i]);
    }
    return result;
}

/**
 * @brief Converts a message array into a polynomial.
 *
 * This function takes an array of unsigned short integers (msg) and 
 * converts it into a polynomial structure by copying the elements 
 * of the array into the coefficients of the polynomial.
 *
 * @param msg A pointer to an array of unsigned short integers representing the message.
 *            The size of the array should be at least PARAMS_N.
 * @return A polynomial structure with its coefficients initialized from the message array.
 */
poly poly_frommsg(const unsigned short *msg) {
    poly result;
    for (int i = 0; i < PARAMS_N; i++) {
        result.coeffs[i] = msg[i];
    }
    return result;
}