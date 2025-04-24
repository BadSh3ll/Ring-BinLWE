#include <stdio.h>
#include <stdlib.h>


void randombytes(unsigned char *x, unsigned long long xlen) {
    FILE* f = fopen("/dev/urandom", "rb");
    if (f == NULL) {
        perror("Failed to open /dev/urandom");
        exit(EXIT_FAILURE);
    }
    size_t bytesRead = fread(x, 1, xlen, f);
    if (bytesRead != xlen) {
        perror("Failed to read random bytes");
        fclose(f);
        exit(EXIT_FAILURE);
    }
    fclose(f);
}

void randombit(unsigned char *x) {
    unsigned char byte;
    randombytes(&byte, 1);
    *x = byte & 1; // Set x to the least significant bit of the byte
}

void sampleBinary(unsigned char *x, unsigned long long xlen) {
    
    for (unsigned long long i = 0; i < xlen; i++) {
        randombit(&x[i]);
    }
}