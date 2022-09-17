#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<stdbool.h>
#include "string_cmp.h"

bool basic_strcmp(char* a, char* b){
    int len = 0;
    while(a[len] == b[len] && a[len] != '\0' && (len++)) {}
    return ((a[len] != '\0')? false : true );
}

int measure_time(FILE* ptr){

	fprintf(ptr, "slen, glibc implementation, custom implementation, linear implementation\n");

	clock_t t;
	double time_taken_ms_a, time_taken_ms_b, time_taken_ms_c;
	bool lena, lenb, lenc;

	for(unsigned long slen = 10000 ; slen < 2500000000 ; slen *= 2){

		//generate input
		char *str1 = (char*) malloc (slen * sizeof(char));
		char *str2 = (char*) malloc (slen * sizeof(char));
		memset(str1, 'a' , slen * sizeof(char));
        memset(str2, 'a' , slen * sizeof(char));

		//measure a and b

		//glibc function
		t = clock();
		lena = strcmp(str1, str2);
		t = clock() - t;
		time_taken_ms_a = ((double)t)*1000/CLOCKS_PER_SEC;
		//assert(len == slen);


		//custom function
		t = clock();
		lenb = string_cmp(str1, str2);
		t = clock() - t;
		time_taken_ms_b = ((double)t)*1000/CLOCKS_PER_SEC;
		//assert(len == slen);

		//linear function
		t = clock();
		lenc = basic_strcmp(str1, str2);
		t = clock() - t;
		time_taken_ms_c = ((double)t)*1000/CLOCKS_PER_SEC;

		
		printf("lena:%i lenb:%i lenc:%i slen: %lu, %f, %f, %f\n",lena, lenb, lenc, slen, time_taken_ms_a, time_taken_ms_b, time_taken_ms_c);
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

	if(NULL == ptr){
		printf("ERROR: result file '%s' can't be opened\n", argv[1]);
		return -1;
	}

	// char* str1 = "hello____________________________________________________________________________";
	// char* str2 = "hello____________________________________________________________________________";
	// char* str3 = "hello_________________________________________________________________________";

	// printf("%i\n", string_cmp(str1, str3));
	// printf(string_cmp(str1, str3));


	printf("Starting benchmarking\n");
	measure_time(ptr);
	printf("Finished!\nBenchmark saved in %s\n", argv[1]);

	return 0;
}
