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

#include "config.h"
#include "speed.h"
#include "fill.h"


FILE *fp;
uint8_t buff[BUFF_SIZE];

int main(int argc, char **argv)
{
	if(argc < 4){
		printf("%d\n", argc);
		return 1;
	}
	fp = fopen(argv[1], "wb+");
	check(fp, atoi(argv[2]), atoi(argv[3]));
	fclose(fp);
}
