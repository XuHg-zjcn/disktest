/*
    Main program of disktest
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
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>
#include <bsd/stdlib.h>

#include "config.h"
#include "speed.h"
#include "fill.h"
#include "disktest_common.h"

#define PARAM_RANDCHK_STEP	0xff01
#define PARAM_RANDCHK_NUM	0xff02
#define PARAM_RANDCHK_SIZE	0xff03
#define PARAM_MODE              0xff04

FILE *fp;
uint8_t buff[BUFF_SIZE];

int64_t fsize = 1073741824; //1GiB
uint64_t base = 0;
bool randchk=true, fullchk=true;
int64_t randchk_step = 536870912; //512MiB，每写入多少数据随机检查一次
uint64_t randchk_num = 100;  //每次随机检查抽查样本个数
int64_t randchk_size = 512; //抽查样本大小
testmode_t mode = {
  .write_en = true,
  .readrand_en = true,
  .readfull_en = true,
  .is_custom = false,
};

static struct option long_options[] = {
  {"seed",         required_argument, 0,  's' },
  {"fsize",        required_argument, 0,  'S' },
  {"randchk",      no_argument,       0,  'r' },
  {"fullchk",      no_argument,       0,  'f' },
  {"no-randchk",   no_argument,       0,  'R' },
  {"no-fullchk",   no_argument,       0,  'F' },
  {"randchk_step", required_argument, 0,  PARAM_RANDCHK_STEP   },
  {"randchk_num",  required_argument, 0,  PARAM_RANDCHK_NUM    },
  {"randchk_size", required_argument, 0,  PARAM_RANDCHK_SIZE   },
  {"mode",         required_argument, 0,  PARAM_MODE           },
  {0,              0,                 0,  0   },
};

int main(int argc, char **argv)
{
  while(1){
    int option_index, c;
    c = getopt_long(argc, argv, "s:S:rfRF", long_options, &option_index);
    if(c == -1){
      break;
    }
    printf("%c", c);
    printf("option %s", long_options[option_index].name);
    if (optarg){
      printf(" with arg %s", optarg);
    }
    printf("\n");
    switch(c){
    case 's':
      sscanf(optarg, "%016lx", &base);
      if(!mode.is_custom){
	mode.write_en = false;
      }
      break;
    case 'S':
      if(dehumanize_number(optarg, &fsize) == -1){
	perror("Error param fsize");
	return 1;
      }
      break;
    case PARAM_RANDCHK_STEP:
      if(dehumanize_number(optarg, &randchk_step)){
	perror("Error param randchk_step");
	return 1;
      }
      break;
    case PARAM_RANDCHK_NUM:
      randchk_num = atoll(optarg);
      break;
    case PARAM_RANDCHK_SIZE:
      if(dehumanize_number(optarg, &randchk_size)){
	perror("Error param randchk_size");
	return 1;
      }
      break;
    case PARAM_MODE:
      mode.is_custom = true;
      mode.write_en = false;
      mode.readrand_en = false;
      mode.readfull_en = false;
      char *p = optarg;
      while(1){
	char stmp[64];
	char *p2 = strchrnul(p, ',');
	strncpy(stmp, p, MAX(0, MIN(p2-p, 64)));
	if(strcmp(p, "write") == 0){
	  mode.write_en = true;
	}else if(strcmp(p, "readrand") == 0 || strncmp(p, "rrand", 5) == 0){
	  mode.readrand_en = true;
	}else if(strcmp(p, "readfull") == 0 || strncmp(p, "rfull", 5) == 0){
	  mode.readfull_en = true;
	}else{
	  fprintf(stderr, "Error param unknown mode:%s\n", p);
	  return 1;
	}
	if(*p2 == '\0'){
	  break;
	}else{
	  p = p2+1;
	}
      }
    default:
      break;
    }
  }
  if (optind < argc) {
    if(mode.write_en){
      fp = fopen(argv[optind], "wb+");
      check_writeread(fp, fsize/randchk_step, randchk_step/BUFF_SIZE);
      fclose(fp);
    }else if(mode.readrand_en || mode.readfull_en){
      fp = fopen(argv[optind], "rb");
      check_readonly(fp, fsize/BUFF_SIZE);
      fclose(fp);
    }else{
      printf("no mode select\n");
    }
  }else{
    printf("need output file name\n");
  }
  return 0;
}
