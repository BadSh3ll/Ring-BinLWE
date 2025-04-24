#ifndef KEM_H
#define KEM_H

void keygen(unsigned short *pk, unsigned short *sk);
void enc(unsigned short *ct, const unsigned short *m, const unsigned short *pk);
void dec(unsigned short *m, const unsigned short *ct, const unsigned short *sk);

#endif // UTILS_H