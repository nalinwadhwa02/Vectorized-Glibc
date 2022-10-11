#include<immintrin.h>

unsigned long check_character_location(char* arr, char c){
	
	unsigned long len = 0;
	int clen_EOS = 0;
    int clen_EOC = 0;

	__m256i mask, vec, EOS, cmp_EOS, EOC, cmp_EOC;

	mask = _mm256_set1_epi32(-1);
	EOS = _mm256_set1_epi8('\0');
    EOC = _mm256_set1_epi8(c);

	do{

		// vec = _mm256_maskload_epi32((int const*)(arr+len), mask);
		vec = _mm256_loadu_si256((const __m256i *)(arr + len));
        cmp_EOC = _mm256_cmpeq_epi8(vec, EOC);
        clen_EOC = _mm256_movemask_epi8(cmp_EOC);
		cmp_EOS = _mm256_cmpeq_epi8(vec, EOS);
		clen_EOS = _mm256_movemask_epi8(cmp_EOS);

	}while((clen_EOC == 0 && clen_EOS == 0) && (len+=32));

	int lC = _tzcnt_u32(clen_EOC);
	int lEOS = _tzcnt_u32(clen_EOS);

	if(lC <= lEOS) len += lC;
	else len += lEOS;

	return len;
}

