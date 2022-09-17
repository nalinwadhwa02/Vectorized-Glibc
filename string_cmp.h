#include<immintrin.h>
#include<stdio.h>

// void print_bytes(void *ptr, int size){
//     unsigned char *p = ptr;
//     int i;
//     for (i=0; i<size; i++) {
//         printf("%02hhX ", p[i]);
//     }
//     printf("\n");
// }

bool string_cmp(char* arr1, char *arr2){
	
	unsigned long len = 0;
	int clen_EOS1 = 0, clen_EOS2 = 0, clen_vec = 0;

	__m256i mask, vec1, vec2, EOS, cmp_vec, cmp_EOS1, cmp_EOS2;

	mask = _mm256_set1_epi32(-1);
	EOS = _mm256_set1_epi8('\0');

	do{
		vec1 = _mm256_maskload_epi32((int const*)(arr1+len), mask);
		vec2 = _mm256_maskload_epi32((int const*)(arr2+len), mask);

		cmp_EOS1 = _mm256_cmpeq_epi8(vec1, EOS);
		clen_EOS1 = _mm256_movemask_epi8(cmp_EOS1);

		cmp_EOS2 = _mm256_cmpeq_epi8(vec2, EOS);
		clen_EOS2 = _mm256_movemask_epi8(cmp_EOS2);

		cmp_vec = _mm256_cmpeq_epi8(vec2, vec1);
		clen_vec = _mm256_movemask_epi8(cmp_vec);
		clen_vec ^= __UINT32_MAX__;

		// print_bytes((void*)&clen_EOS1, 4);
		// print_bytes((void*)&clen_EOS2, 4);
		// print_bytes((void*)&clen_vec, 4);
		// printf("%i %i %i\n", clen_EOS1, clen_EOS2, clen_vec);
		// printf("%i %i %i\n", _tzcnt_u32(clen_EOS1), _tzcnt_u32(clen_EOS2), _tzcnt_u32(clen_vec));


	}while((clen_EOS2 == 0 && clen_EOS1 == 0 && clen_vec == 0) && (len+=32));

	if(clen_EOS2 == 0 && clen_EOS1 == 0)
		return false;
	else if(clen_EOS2 != clen_EOS1)
		return false;
	else if(_tzcnt_u32(clen_EOS2) > _tzcnt_u32(clen_vec))
		return false;

	return true;
}


