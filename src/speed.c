#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/syscall.h>

#include "config.h"
#include "speed.h"

extern uint8_t buff[BUFF_SIZE];

int64_t write_rand(FILE *fp, int Nblock)
{
	unsigned int seed;
	struct timespec ts1, ts2;
	syscall(SYS_getrandom, &seed, sizeof(seed), 0);
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
