#include <immintrin.h>

void strconcatblock(char* a, char* b){
    unsigned int alen = 0;
    int clen = 0;

    __m256i mask, vec, EOS, cmp;

    mask = _mm256_set1_epi32(-1);
    EOS = _mm256_set1_epi8('\0');

    do{
        vec = _mm256_maskload_epi32((int const*) (a+alen), mask);
        cmp = _mm256_cmpeq_epi32(vec, EOS);
        clen = _mm256_movemask_epi8(cmp);
    }while((clen == 0) && (alen+=32));

    alen += _tzcnt_u32(clen);

    unsigned int blen = 0;
    do{
        vec = _mm256_maskload_epi32((int const*) (b + blen), mask);
        cmp = _mm256_cmpeq_epi32(vec, EOS);
        clen = _mm256_movemask_epi8(cmp);
    }while((clen == 0) && (_mm256_maskstore_epi32((int const*) (a+alen+blen), mask, vec)) && (blen+=32));

    // blen += _tzcnt_u32(clen);

}