#ifndef SPEED_H
#define SPEED_H

int64_t write_rand_lib(FILE *fp, int size);
int64_t write_rand_xor(FILE *fp, int size);
int64_t write_byte(FILE *fp, int Nblock, uint8_t ch);

#endif
