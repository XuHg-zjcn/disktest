#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define RAND_WIDTH __builtin_popcountl(RAND_MAX)

uint64_t uniform(uint64_t max_v)
{
  uint64_t nbit = 64 - __builtin_clzll(max_v);
  uint64_t ret;
  do{
    ret = 0;
    while(nbit > RAND_WIDTH){
      nbit -= RAND_WIDTH;
      ret |= (uint64_t)rand() << nbit;
    }
    uint64_t mask = (1UL << (nbit+1)) - 1;
    ret |= rand() & mask;
  }while(ret > max_v);
  return ret;
}
