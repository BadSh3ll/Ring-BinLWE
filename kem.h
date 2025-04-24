#ifndef KEM_H
#define KEM_H

void keygen(unsigned char *pk, unsigned char *sk);
void enc(unsigned char *ct, const unsigned char *m, const unsigned char *pk);
void dec(unsigned char *m, const unsigned char *ct, const unsigned char *sk);

#endif // UTILS_H