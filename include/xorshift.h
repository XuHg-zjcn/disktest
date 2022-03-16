#ifndef XORSHIFT_H
#define XORSHIFT_H

//ref: https://stackoverflow.com/questions/53886131/how-does-xorshift32-works

#include <inttypes.h>

typedef struct {
    uint64_t  state;
} prng_state;

static inline uint64_t prng_u64(prng_state *const p)
{
    uint64_t  state = p->state;
    state ^= state >> 12;
    state ^= state << 25;
    state ^= state >> 27;
    p->state = state;
    return state * UINT64_C(2685821657736338717);
}

#endif
