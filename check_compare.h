#include<immintrin.h>

unsigned int check_substring(char* arr, char* c32){

	unsigned long len = 0;
	unsigned int clen = 0;

	__m256i mask, vec, cmp, C32;

	mask = _mm256_set1_epi32(-1);
    C32 = _mm256_maskload_epi32((int const*)c32, mask);

	do{

		vec = _mm256_maskload_epi32((int const*)(arr+len), mask);
        cmp = _mm256_cmpeq_epi8(vec, C32);
        clen = _mm256_movemask_epi8(cmp);

	}while(clen == 0 && (len+=32));

	return clen;
}

