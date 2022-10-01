#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include "../check_character_location.h"

unsigned long basic_strlen(char * c){
	unsigned long len = 0;
	while(c[len++]){}
	return len;
}

int measure_time(FILE* ptr){

	fprintf(ptr, "slen, glibc implementation, custom implementation, linear implementation\n");

	clock_t t;
	double time_taken_ms_a, time_taken_ms_b, time_taken_ms_c;
	unsigned long lena, lenb, lenc;

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
		lenb = check_character_location(str, '\0');
		t = clock() - t;
		time_taken_ms_b = ((double)t)*1000/CLOCKS_PER_SEC;
		//assert(len == slen);

		//linear function
		t = clock();
		lenc = basic_strlen(str);
		t = clock() - t;
		time_taken_ms_c = ((double)t)*1000/CLOCKS_PER_SEC;

		
		printf("lena:%lu lenb:%lu lenc:%lu slen: %lu, %f, %f, %f\n",lena, lenb, lenc, slen, time_taken_ms_a, time_taken_ms_b, time_taken_ms_c);
		fprintf(ptr, "%lu, %f, %f, %f\n", slen, time_taken_ms_a, time_taken_ms_b, time_taken_ms_c);
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

	if(ptr == NULL){
		printf("ERROR: result file '%s' can't be opened\n", argv[1]);
		return -1;
	}

	printf("Starting benchmarking\n");
	measure_time(ptr);
	printf("Finished!\nBenchmark saved in %s\n", argv[1]);

	return 0;
}
