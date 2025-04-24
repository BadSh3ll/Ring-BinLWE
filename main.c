#include <stdio.h>
#include <string.h>

#include "kem.h"
#include "utils.h"
#include "params.h"
#include "random.h"

int main() {

    int fails = 0;
    for (int i = 0; i < 100; i++) {
        
        unsigned short pk[PARAMS_N * 2];
        unsigned short sk[PARAMS_N];
    
        // Key generation
        keygen(pk, sk);
    
        unsigned short m1[PARAMS_N];
        sampleBinary(m1, PARAMS_N);
    
        // Encryption
        unsigned short ct[PARAMS_N * 2];
        enc(ct, m1, pk);
    
        // Decryption
        unsigned short m2[PARAMS_N];
        dec(m2, ct, sk);
        if (memcmp(m1, m2, PARAMS_N) != 0) {
            fails++;
        } 
    }
    if (fails == 0) {
        printf("All tests passed!\n");
    } else {
        printf("%d tests failed!\n", fails);
    }
    return 0;


}