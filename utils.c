#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "params.h"
#include "poly.h"

void print(unsigned short *x, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d ", x[i]);
    }
    printf("\n");
}
 
/**
 * @brief Reduces an integer to a value within the range [0, PARAMS_Q).
 *
 * This function performs modular reduction using Barrett reduction. It ensures
 * that the input integer `x` is reduced to a value within the range [0, PARAMS_Q).
 *
 * @param x The input integer to be reduced. It is expected to be a signed 16-bit integer.
 * @return The reduced value as an unsigned 16-bit integer within the range [0, PARAMS_Q).
 *
 * @note The constants `BARRETT_MU`, `BARRETT_K`, and `PARAMS_Q` must be defined
 *       appropriately for the Barrett reduction to work correctly.
 */
uint16_t reduce(int16_t x) {
    int64_t t = ((int64_t)x * BARRETT_MU) >> BARRETT_K;
    int32_t res = x - t * PARAMS_Q;
    if (res < 0)
        res += PARAMS_Q;
    else if (res >= PARAMS_Q)
        res -= PARAMS_Q;
    return (uint16_t)res;
}



/**
 * @brief Encodes a message into a polynomial representation.
 *
 * This function takes a message represented as an array of unsigned short
 * integers and encodes it into a polynomial structure. Each coefficient of
 * the polynomial is set to HALF_Q if the corresponding message bit is non-zero,
 * otherwise it is set to 0.
 *
 * @param[out] mhat Pointer to the polynomial structure where the encoded message will be stored.
 * @param[in] msg Pointer to the array of unsigned short integers representing the message.
 *
 * @note The size of the message array and the polynomial coefficients is assumed
 *       to be PARAMS_N.
 */
void encode_m(poly *mhat, const unsigned short *msg) {
    for (int i = 0; i < PARAMS_N; i++) {
        mhat->coeffs[i] = msg[i] ? HALF_Q : 0;
    }
}


/**
 * @brief Decodes a polynomial into a binary message.
 *
 * This function takes a polynomial `mhat` and decodes it into a binary
 * message `msg` based on the proximity of each coefficient to `HALF_Q`.
 * If a coefficient is within a range of `Q4` from `HALF_Q`, the corresponding
 * message bit is set to 1; otherwise, it is set to 0.
 *
 * @param[out] msg   Pointer to an array of unsigned shorts where the decoded
 *                   binary message will be stored. The array must have at
 *                   least `PARAMS_N` elements.
 * @param[in]  mhat  Pointer to a polynomial structure containing the coefficients
 *                   to be decoded.
 */
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

/**
 * @brief Encodes the public key by concatenating two polynomials.
 *
 * This function takes two input polynomials, `a` and `p`, and encodes them
 * into a single array `pk` by concatenating their coefficients. The first
 * half of `pk` contains the coefficients of `a`, and the second half contains
 * the coefficients of `p`.
 *
 * @param[out] pk Pointer to the output array where the encoded public key will be stored.
 *                The array must have a size of at least 2 * PARAMS_N.
 * @param[in]  a  Pointer to the first input polynomial.
 * @param[in]  p  Pointer to the second input polynomial.
 */
void encode_p(unsigned short *pk, const poly *a, const poly *p) {
    for (int i = 0; i < PARAMS_N; i++) {
        pk[i] = a->coeffs[i];
        pk[i+PARAMS_N] = p->coeffs[i];
    }
}

/**
 * @brief Decodes a public key into two polynomials.
 *
 * This function takes a public key represented as an array of unsigned shorts
 * and splits it into two polynomials, `a` and `p`. The first half of the public
 * key is assigned to the coefficients of polynomial `a`, and the second half
 * is assigned to the coefficients of polynomial `p`.
 *
 * @param[out] a   Pointer to the first polynomial to store the decoded coefficients.
 * @param[out] p   Pointer to the second polynomial to store the decoded coefficients.
 * @param[in]  pk  Pointer to the public key array containing the coefficients.
 */
void decode_p(poly *a, poly *p, const unsigned short *pk) {
    for (int i = 0; i < PARAMS_N; i++) {
        a->coeffs[i] = pk[i];
        p->coeffs[i] = pk[i+PARAMS_N];
    }
}


/**
 * @brief Encodes two polynomials into a single array of unsigned shorts.
 *
 * This function takes two input polynomials, `c1` and `c2`, and combines their
 * coefficients into a single array `ct`. The coefficients of `c1` are stored
 * in the first half of `ct`, and the coefficients of `c2` are stored in the
 * second half of `ct`.
 *
 * @param c1 Pointer to the first polynomial (of type `poly`).
 * @param c2 Pointer to the second polynomial (of type `poly`).
 * @param ct Pointer to the output array of unsigned shorts, where the encoded
 *           coefficients will be stored. The array must have a size of at least
 *           `2 * PARAMS_N`.
 */
void encode_c(const poly *c1, const poly *c2, unsigned short *ct) {
    for (int i = 0; i < PARAMS_N; i++) {
        ct[i] = c1->coeffs[i];
        ct[i+PARAMS_N] = c2->coeffs[i];
    }
}


/**
 * @brief Decodes a ciphertext array into two polynomial structures.
 *
 * This function takes an input ciphertext array `ct` and splits it into
 * two polynomials `c1` and `c2`. The first half of the array is assigned
 * to `c1->coeffs`, and the second half is assigned to `c2->coeffs`.
 *
 * @param[out] c1 Pointer to the first polynomial structure to store the decoded coefficients.
 * @param[out] c2 Pointer to the second polynomial structure to store the decoded coefficients.
 * @param[in]  ct Pointer to the input ciphertext array containing the coefficients.
 */
void decode_c(poly *c1, poly *c2, const unsigned short *ct) {
    for (int i = 0; i < PARAMS_N; i++) {
        c1->coeffs[i] = ct[i];
        c2->coeffs[i] = ct[i+PARAMS_N];
    }
}

