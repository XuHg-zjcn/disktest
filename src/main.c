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
