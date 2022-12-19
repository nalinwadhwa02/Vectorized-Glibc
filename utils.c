#include <stdio.h>
#include <stdint.h>
#ifndef UTILS_H
#define UTILS_H

void print_bytes(char *ptr, int size){
	int i=0;
	while(i<size)
		printf("%x ",*(ptr+(i++)));
	printf("\n");
}

void print256_num(__m256i var)
{
    char val[32];
    memcpy(val, &var, sizeof(val));
	printf("Numerical: ");
	for(int i = 0; i < 32; i ++){
		printf("%i ", val[i] == 0 ? 0 : 1);
	}
	printf("\n");
}

#endif