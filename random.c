#include <stdio.h>
#include <stdlib.h>

#include "utils.h"


/**
 * @brief Generates an array of random unsigned short integers and reduces each value.
 *
 * This function reads random data from the system's `/dev/urandom` device and fills
 * the provided array with random unsigned short integers. After reading the random
 * values, it applies a reduction operation to each element in the array using the
 * `reduce` function.
 *
 * @param x Pointer to an array of unsigned short integers where the random values
 *          will be stored.
 * @param xlen The number of elements in the array to be filled with random values.
 *
 * @note The `reduce` function must be defined elsewhere in the program. It is used
 *       to process each random value after it is read.
 *
 * @warning If the function fails to open `/dev/urandom` or read the required number
 *          of random bytes, it will print an error message and terminate the program
 *          using `exit(EXIT_FAILURE)`.
 */
void GenA(unsigned short *x, unsigned long long xlen) {
    FILE* f = fopen("/dev/urandom", "rb");
    if (f == NULL) {
        perror("Failed to open /dev/urandom");
        exit(EXIT_FAILURE);
    }
    size_t bytesRead = fread(x, sizeof(unsigned short), xlen, f);
    if (bytesRead != xlen) {
        perror("Failed to read random bytes");
        fclose(f);
        exit(EXIT_FAILURE);
    }
    fclose(f);

    for (unsigned long long i = 0; i < xlen; i++) {
        x[i] = reduce(x[i]);
    }
}

/**
 * @brief Samples random binary values (0 or 1) and stores them in the provided array.
 *
 * This function reads random bytes from the system's cryptographic random number generator
 * (/dev/urandom) and extracts the least significant bit (LSB) from each byte to generate
 * binary values (0 or 1). The binary values are stored in the provided array.
 *
 * @param x Pointer to an array of unsigned short where the sampled binary values will be stored.
 * @param xlen The number of binary values to sample and store in the array.
 *
 * @note The function will terminate the program with an error message if it fails to open
 *       /dev/urandom or read random bytes from it.
 *
 * @warning Ensure that the array pointed to by `x` has enough space to store `xlen` binary values.
 */
void sampleBinary(unsigned short *x, unsigned long long xlen) {
    FILE* f = fopen("/dev/urandom", "rb");
    if (f == NULL) {
        perror("Failed to open /dev/urandom");
        exit(EXIT_FAILURE);
    }

    for (unsigned long long i = 0; i < xlen; i++) {
        unsigned short byte;
        size_t bytesRead = fread(&byte, 1, 1, f);
        if (bytesRead != 1) {
            perror("Failed to read random byte");
            fclose(f);
            exit(EXIT_FAILURE);
        }
        x[i] = byte & 1; // Extract the least significant bit (0 or 1)
    }

    fclose(f);
}