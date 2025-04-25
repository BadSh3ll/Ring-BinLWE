#ifndef PARAMS_H
#define PARAMS_H


#define PARAMS_N 1024
#define PARAMS_Q 12289
#define HALF_Q (PARAMS_Q / 2)
#define Q4 (PARAMS_Q / 4)

#define BARRETT_K 32
#define BARRETT_MU ((uint32_t)(((uint64_t)1 << BARRETT_K) / PARAMS_Q))

#endif