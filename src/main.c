/*
    Main program of disktest
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
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include "config.h"
#include "speed.h"
#include "fill.h"


FILE *fp;
uint8_t buff[BUFF_SIZE];

uint64_t fsize = 1073741824; //1GiB
uint64_t base = 0;
bool randchk=true, fullchk=true;
uint64_t randchk_step = 536870912; //512MiB
uint64_t randchk_num = 100;
uint64_t randchk_size = 512;

static struct option long_options[] = {
  {"seed",         required_argument, 0,  's' },
  {"fsize",        required_argument, 0,  'S' },
  {"randchk",      no_argument,       0,  'r' },
  {"fullchk",      no_argument,       0,  'f' },
  {"no-randchk",   no_argument,       0,  'R' },
  {"no-fullchk",   no_argument,       0,  'F' },
  {"randchk_step", required_argument, 0,  0   },
  {"randchk_num",  required_argument, 0,  0   },
  {"randchk_size", required_argument, 0,  0   },
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
    //printf("option %s", long_options[option_index].name);
    if (optarg){
      printf(" with arg %s", optarg);
    }
    printf("\n");
    switch(c){
    case 's':
      sscanf(optarg, "%016lx", &base);
      break;
    case 'S':
      fsize = atoll(optarg);
      break;
    }
  }
  return 0;
}
