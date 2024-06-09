/*
    Fill data to test capacity
    Copyright (C) 2022-2024  Xu Ruijun

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
#include "fill.h"
#include "checksum.h"
#include "gentab64.inc"
#include "config.h"
#include <time.h>
#include <unistd.h>
#include <sys/random.h>
#include "myrandom.h"
#include "disktest_common.h"

extern testmode_t mode;
extern uint8_t buff[BUFF_SIZE];
extern uint64_t base;
extern uint64_t randchk_num;
extern uint64_t randchk_size;
#define buff64 ((uint64_t *)buff)

void fill_crc64_ecma(uint64_t base, uint64_t *output, uint64_t n)
{
  uint64_t crc;
  while(n--){
	crc = CRC_START_64_ECMA;
	for(int i=0;i<8;i++){
	  //没有经过严格数学验证，不知道是否容易破解。
	  //不要用大端值进行CRC，否则测试文件压缩一下就很小了。
	  crc = (crc << 8) ^ crc_tab64[ ((crc >> 56) ^ ((base>>(i*8))&0xff)) & 0xff];
	}
	*output++ = crc;
	base++;
  }
}

uint64_t check_crc64_ecma(uint64_t base, uint64_t *input, uint64_t n)
{
  uint64_t errs = 0;
  uint64_t crc;
  while(n--){
	crc = CRC_START_64_ECMA;
	for(int i=0;i<8;i++){
	  crc = (crc << 8) ^ crc_tab64[ ((crc >> 56) ^ ((base>>(i*8))&0xff)) & 0xff];
	}
	if(*input++ != crc){
	  errs++;
	}
	base++;
  }
  return errs;
}

void fill_fp(FILE *fp, uint64_t base, int Nblock)
{
  while(Nblock--){
	fill_crc64_ecma(base, buff64, BUFF_SIZE/8);
	base += BUFF_SIZE/8;
    fwrite(buff, 1, BUFF_SIZE, fp);
  }
  fflush(fp);
  fsync(fileno(fp));
}

uint64_t check_fp(FILE *fp, uint64_t base, int Nblock)
{
  uint64_t errs = 0;
  while(Nblock--){
    fread(buff, 1, BUFF_SIZE, fp);
	errs += check_crc64_ecma(base, buff64, BUFF_SIZE/8);
	base += BUFF_SIZE/8;
  }
  return errs;
}

uint64_t random_check(FILE *fp, uint64_t base, int Npoint, int rep)
{
  uint64_t errs = 0;
  long old = ftell(fp);
  while(Npoint--){
    long offset = uniform(old/8);
    fseek(fp, offset*8, SEEK_SET);
    long rep_ = (offset+rep>old/8)?(old/8-offset):rep;
    fread(buff, 8, rep_, fp);
    errs += check_crc64_ecma(base+offset, buff64, rep_);
  }
  fseek(fp, old, SEEK_SET);
  return errs;
}

//TODO: print输出错误位置
void check_writeread(FILE *fp, int Ncyc, int Nchk)
{
  uint64_t errs=0, rerrs=0;
  uint64_t hashs=0;
  if(!base){
    getrandom(&base, sizeof(base), 0);
    printf("%16lx\n", base);
  }
  for(int i=0;i<Ncyc;i++){
    fill_fp(fp, base+hashs, Nchk);
    hashs += Nchk*BUFF_SIZE/8;
    if(mode.readrand_en){
      uint64_t rerr = random_check(fp, base, randchk_num, randchk_size);
      rerrs += rerr;
      if(rerr != 0){
	printf("rerr=%ld\n", rerrs);
      }
    }
  }
  if(mode.readrand_en){
    printf("rand check rerrs=%ld\n", rerrs);
  }
  if(mode.readfull_en){
    fseek(fp, 0, SEEK_SET);
    errs = check_fp(fp, base, Nchk*Ncyc);
    printf("full check errs=%ld\n", errs);
  }
}

void check_readonly(FILE *fp, int Nblock)
{
  if(!base){
    fprintf(stderr, "readonly mode must provide seed\n");
    return;
  }
  if(mode.readrand_en){
    fseek(fp, 0, SEEK_END);
    uint64_t rerrs = random_check(fp, base, randchk_num, randchk_size);
    printf("rand check rerrs=%ld\n", rerrs);
  }
  if(mode.readfull_en){
    fseek(fp, 0, SEEK_SET);
    uint64_t errs = check_fp(fp, base, Nblock);
    printf("full check errs=%ld\n", errs);
  }
}
