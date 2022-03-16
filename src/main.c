#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#include "config.h"
#include "speed.h"


FILE *fp;
uint8_t buff[BUFF_SIZE];

int main(int argc, char **argv)
{
	if(argc < 3){
		printf("%d\n", argc);
		return 1;
	}
	int block=atoi(argv[2]);
	fp = fopen(argv[1], "wb");
	int64_t ns = write_rand(fp, block);
	printf("%ldns, %lf\n", ns, (double)block*BUFF_SIZE/ns);
}
