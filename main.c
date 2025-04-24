#include <stdio.h>
#include <string.h>

#include "kem.h"
#include "utils.h"
#include "params.h"
#include "random.h"

int main() {

    int fails = 0;
    for (int i = 0; i < 100; i++) {
        
        // Public key pk and secret key sk
        unsigned char pk[PARAMS_N*2];
        unsigned char sk[PARAMS_N];
        // Generate the public and secret keys
        keygen(pk, sk);
        
        // Shared secret m1
        unsigned char m1[PARAMS_N];
        sampleBinary(m1, PARAMS_N);
        // Encyption
        unsigned char ct[2*PARAMS_N];
        enc(ct, m1, pk);
        
        // Decryption
        unsigned char m2[PARAMS_N];
        dec(m2, ct, sk);
        // Check if the decrypted message matches the original message
        if (memcmp(m1, m2, PARAMS_N) != 0) {
            fails++;
        }
    }
    if (fails == 0) {
        printf("All tests passed!\n");
    } else {
        printf("%d tests failed.\n", fails);
    }
    printf("Done!\n");


    return 0;


}