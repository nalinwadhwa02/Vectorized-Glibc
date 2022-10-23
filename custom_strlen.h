#include<immintrin.h>
#include<time.h>
#include<stdio.h>
#include<string.h>

/*helper function (not needed)
void print_bytes(void *ptr, int size){
    unsigned char *p = ptr;
    int i;
    for (i=0; i<size; i++) {
        printf("%02hhX ", p[i]);
    }
    printf("\n");
}
*/

unsigned long custom_strlen(char* arr){
	
	unsigned long len = 0;
	int clen = 0;

	__m256i mask, vec, EOS, cmp;

	mask = _mm256_set1_epi32(-1);
	EOS = _mm256_set1_epi8('\0');

	do{

		vec = _mm256_maskload_epi32((int const*)(arr+len), mask);
		cmp = _mm256_cmpeq_epi8(vec, EOS);
		clen = _mm256_movemask_epi8(cmp);

	}while(clen == 0 && (len+=32));

	len += _tzcnt_u32(clen);
	// print_bytes((void *) &cmp, 32);
	// printf("%d\n", len);

	return len;
}
/*
int main(int agc, char* argv[]){
	
	//testing the function :D
	char str[] = "ttttttttttttttttttttthhhhhhhhhhhhhhhhhhhhhhhhhhhhiiiiiiiiiiiiiiiiiiiiiiisssssssssssssssssssss iiiiiiiissssssssssss llllllllloooooooooooonnnnnnnnnggggggggggg :)";
	char str1[] = "is this working or not?          ";
	char str2[] = "woah this works !!! :)";
	char str3[] = "";
	char str4[] = "f";
	char str5[] = "ff";
	char str6[] = "fff";

	char *str_sp = (char *) malloc(10000000 * sizeof(char));
	memset(str_sp, 'a', 10000000 * sizeof(char));


	clock_t t;
	t = clock();
	unsigned long len = custom_strlen(str_sp);
	t = clock() - t;
	double time_taken_ms = ((double)t)*1000/CLOCKS_PER_SEC;

	printf("len: %lu\ntime_taken_ms: %f", len, time_taken_ms);

	//printf("'%s' %lu\n", str, custom_strlen(str));
	//printf("'%s' %lu\n", str1, custom_strlen(str1));
	//printf("'%s' %lu\n", str2, custom_strlen(str2));
	//printf("'%s' %lu\n", str3, custom_strlen(str3));
	//printf("'%s' %lu\n", str4, custom_strlen(str4));
	//printf("'%s' %lu\n", str5, custom_strlen(str5));
	//printf("'%s' %lu\n", str6, custom_strlen(str6));


	return 0;
}
*/
