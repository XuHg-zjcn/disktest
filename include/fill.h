#ifndef FILL_H
#define FILL_H

#include <stdio.h>
#include <stdint.h>

void fill_fp(FILE *fp, uint64_t base, int Nblock);
uint64_t check_fp(FILE *fp, uint64_t base, int Nblock);
uint64_t random_check(FILE *fp, uint64_t base, int Npoint, int rep);
void check(FILE *fp, int Ncyc, int Nchk);

#endif
