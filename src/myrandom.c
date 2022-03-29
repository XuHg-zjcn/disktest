/*
    Generate random data
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
