#include "fill.h"
#include "checksum.h"
#include "gentab64.inc"
#include "config.h"
#include <time.h>
#include <unistd.h>
#include <sys/random.h>
#include "myrandom.h"

extern uint8_t buff[BUFF_SIZE];
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

void check(FILE *fp, int Ncyc, int Nchk)
{
  uint64_t errs = 0;
  uint64_t base;
  uint64_t hashs = 0;
  getrandom(&base, sizeof(base), 0);
  printf("%16lx\n", base);
  for(int i=0;i<Ncyc;i++){
    fill_fp(fp, base+hashs, Nchk);
    hashs += Nchk*BUFF_SIZE/8;
	uint64_t rerrs = random_check(fp, base, hashs/100000, 128);
	if(rerrs != 0){
	  printf("err\n");
	}
  }
  fseek(fp, 0, SEEK_SET);
  errs = check_fp(fp, base, Nchk*Ncyc);
  printf("check errs=%ld\n", errs);
}
