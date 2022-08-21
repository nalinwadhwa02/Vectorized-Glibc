#include<immintrin.h>
#include<stdio.h>

void print_bytes(void *ptr, int size) 
{
    unsigned char *p = ptr;
    int i;
    for (i=0; i<size; i++) {
        printf("%02hhX ", p[i]);
    }
    printf("\n");
}

unsigned long custom_strlen(char* arr){
	
	unsigned long len = 0;

	__m256i mask = _mm256_setr_epi32(-1, -1, -1, -1, -1, -1, -1, -1);

	__m256i vec = _mm256_maskload_epi32((int const*)arr, mask);

	__m256i EOS = _mm256_set1_epi8('\0');

	__m256i cmp = _mm256_cmpeq_epi8(vec, EOS);

	len = _tzcnt_u32(_mm256_movemask_epi8(cmp));

	// char* res = (char*) &cmp;
	// print_bytes((void *) &cmp, 32);
	// printf("%d\n", _tzcnt_u32(len));

	return len;
}

int main(int agc, char* argv[]){
	
	int arr16[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	int arr5[5] = {0, 1, 2, 3, 4};

	char str[] = "nalin wadhwa";
	char str1[] = "is this working or not?";
	char str2[] = "woah this works !!! :)";
	char str3[] = "";
	char str4[] = "f";
	char str5[] = "ff";
	char str6[] = "fff";

	printf("'%s' %lu\n", str, custom_strlen(str));
	custom_strlen(str1);
	custom_strlen(str2);
	custom_strlen(str3);
	custom_strlen(str4);
	custom_strlen(str5);
	custom_strlen(str6);


	return 0;
}
