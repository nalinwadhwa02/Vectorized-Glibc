#include<immintrin.h>
// #include<stdio.h>

// void print_bytes(char *ptr, int size){
// 	int i=0;
// 	while(i<size)
// 		printf("%x ",*(ptr+(i++)));
// 	printf("\n");
// }

int string_cmp(char* arr1, char *arr2){
	
	unsigned long len = 0;
	int clen_EOS1 = 0, clen_EOS2 = 0, clen_vec = 0;

	__m256i mask, vec1, vec2, EOS, cmp_vec, cmp_EOS1, cmp_EOS2;

	mask = _mm256_set1_epi32(-1);
	EOS = _mm256_set1_epi32('\0');

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

		// print_bytes((char*)&vec1, 32);
		// print_bytes((char*)&vec2, 32);

		// print_bytes((char*)&clen_EOS1, 4);
		// print_bytes((char*)&clen_EOS2, 4);
		// print_bytes((char*)&clen_vec, 4);
		// printf("%i %i %i\n", clen_EOS1, clen_EOS2, clen_vec);
		// printf("%i %i %i\n", _tzcnt_u32(clen_EOS1), _tzcnt_u32(clen_EOS2), _tzcnt_u32(clen_vec));


	}while((clen_EOS2 == 0 && clen_EOS1 == 0 && clen_vec == 0) && (len+=32));

	int d1 = _tzcnt_u32(clen_EOS1), d2 = _tzcnt_u32(clen_EOS2), d3 = _tzcnt_u32(clen_vec);

	if(d3 < d1 && d3 < d2){
		return ((*((char*)&vec1 + d3) > *((char*)&vec2 + d3)) * 2) - 1;
	}
	return 0;

}


