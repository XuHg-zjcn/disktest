/*
    Disk speed test
    Copyright (C) 2022  Xu Ruijun

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/random.h>

#include "config.h"
#include "speed.h"
#include "xorshift.h"

extern uint8_t buff[BUFF_SIZE];

int64_t write_rand_lib(FILE *fp, int Nblock)
{
	unsigned int seed;
	struct timespec ts1, ts2;
	getrandom(&seed, sizeof(seed), 0);
	srand(seed);
	clock_gettime(CLOCK_MONOTONIC, &ts1);
	while(Nblock--){
#if (RAND_MAX < 65535) || (BUFF_SIZE%2!=0)
		for(int i=0;i<BUFF_SIZE;i++){
			buff[i] = rand()&0xff;
		}
#else
		for(int i=0;i<BUFF_SIZE/2;i++){
			((uint16_t *)buff)[i] = rand()&0xffff;
		}
#endif
		fwrite(buff, 1, BUFF_SIZE, fp);
	}
	fflush(fp);
	fsync(fileno(fp));
	clock_gettime(CLOCK_MONOTONIC, &ts2);
	return (ts2.tv_nsec-ts1.tv_nsec) + (ts2.tv_sec-ts1.tv_sec)*1000000000;
}

int64_t write_rand_xor(FILE *fp, int Nblock)
{
	prng_state prng;
	struct timespec ts1, ts2;
	getrandom(&prng.state, sizeof(prng.state), 0);
	clock_gettime(CLOCK_MONOTONIC, &ts1);
	while(Nblock--){
		for(int i=0;i<BUFF_SIZE/8;i++){
			((uint64_t *)buff)[i] = prng_u64(&prng);
		}
		fwrite(buff, 1, BUFF_SIZE, fp);
	}
	fflush(fp);
	fsync(fileno(fp));
	clock_gettime(CLOCK_MONOTONIC, &ts2);
	return (ts2.tv_nsec-ts1.tv_nsec) + (ts2.tv_sec-ts1.tv_sec)*1000000000;
}

int64_t write_byte(FILE *fp, int Nblock, uint8_t ch)
{
	struct timespec ts1, ts2;
	memset(buff, ch, BUFF_SIZE);
	clock_gettime(CLOCK_MONOTONIC, &ts1);
	while(Nblock--){
		fwrite(buff, 1, BUFF_SIZE, fp);
	}
	fflush(fp);
	clock_gettime(CLOCK_MONOTONIC, &ts2);
	return (ts2.tv_nsec-ts1.tv_nsec) + (ts2.tv_sec-ts1.tv_sec)*1000000000;
}
