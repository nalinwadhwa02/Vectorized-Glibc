#include<immintrin.h>
#include<stdio.h>


unsigned long custom_strlen(int* arr){
	
	int len = 0;

	__m256i mask = _mm256_setr_epi32(-1, -1, -1, -1, -1, -1, -1, -1);

	__m256i result = _mm256_maskload_epi32(arr, mask);

	int* res = (int*) &result;
	for(unsigned short l = 0 ; l < 8 ; l ++){
		printf("%d ", res[l]);
	}
	printf("\n");

	return 0;
}

int main(int agc, char* argv[]){
	
	int arr16[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	int arr5[5] = {0, 1, 2, 3, 4};

	custom_strlen(arr16);

	return 0;
}
