#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<assert.h>
#include "custom_strlen.h"

int measure_time(FILE* ptr){

	fprintf(ptr, "slen, glibc implementation, custom implementation\n");

	clock_t t;
	double time_taken_ms_a, time_taken_ms_b;
	unsigned long len;

	for(unsigned long slen = 1000 ; slen < 100000000000 ; slen *= 10){

		//generate input
		char *str = (char*) malloc (slen * sizeof(char));
		memset(str, 'a' , slen * sizeof(char));

		//measure a and b

		//glibc function
		t = clock();
		len = strlen(str);
		t = clock() - t;
		time_taken_ms_a = ((double)t)*1000/CLOCKS_PER_SEC;
		printf("len: %lu\n", len);
		assert(len == slen);


		//custom function
		t = clock();
		len = custom_strlen(str);
		t = clock() - t;
		time_taken_ms_a = ((double)t)*1000/CLOCKS_PER_SEC;
		printf("len: %lu\n", len);
		assert(len == slen);
		
		printf("slen: %lu, %f, %f\n", slen, time_taken_ms_a, time_taken_ms_b);
		fprintf(ptr, "%lu, %f, %f\n", slen, time_taken_ms_a, time_taken_ms_b);
	}

	return 0;
}

int main(int argc, char* argv[]){

	if(argc < 2){
		printf("ERROR: missing result file name\n");
		return -1;
	}

	FILE* ptr;
	ptr = fopen(argv[1], "w");

	if(NULL == ptr){
		printf("ERROR: result file '%s' can't be opened\n", argv[1]);
		return -1;
	}

	printf("Starting benchmarking\n");
	measure_time(ptr);
	printf("Finished!\nBenchmark saved in %s\n", argv[1]);

	return 0;
}
