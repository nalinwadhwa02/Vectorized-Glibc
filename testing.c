#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<assert.h>
#include "custom_strlen.h"

int measure_time(FILE* ptr){

	fprintf(ptr, "slen, glibc implementation, custom implementation\n");

	clock_t t;
	double time_taken_ms_a, time_taken_ms_b;
	unsigned long lena, lenb;

	for(unsigned long slen = 10000 ; slen < 5000000000 ; slen *= 2){

		//generate input
		char *str = (char*) malloc (slen * sizeof(char));
		memset(str, 'a' , slen * sizeof(char));

		//measure a and b

		//glibc function
		t = clock();
		lena = strlen(str);
		t = clock() - t;
		time_taken_ms_a = ((double)t)*1000/CLOCKS_PER_SEC;
		//assert(len == slen);


		//custom function
		t = clock();
		lenb = custom_strlen(str);
		t = clock() - t;
		time_taken_ms_b = ((double)t)*1000/CLOCKS_PER_SEC;
		//assert(len == slen);
		
		printf("lena:%lu lenb:%lu slen: %lu, %f, %f\n",lena, lenb, slen, time_taken_ms_a, time_taken_ms_b);
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
