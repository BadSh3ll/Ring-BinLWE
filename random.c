#include <stdio.h>
#include <stdlib.h>

#include "utils.h"


void randombytes(unsigned short *x, unsigned long long xlen) {
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