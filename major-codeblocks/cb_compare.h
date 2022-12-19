#include <immintrin.h>
#include <stdbool.h>
#include "../utils.c"

unsigned long cb_search (char *a, char *b) {

    unsigned long len = 0;
    
    unsigned int clen_EOS = 0, clen_EOC = 0;
    __m256i mask, vec, EOS, cmp_EOS, EOC, cmp_EOC; 
    
    mask = _mm256_set1_epi32(-1);
    EOS = _mm256_set1_epi8('\0');
    EOC = _mm256_set1_epi8(b);
    bool found = false;

    while(true){
        vec = _mm256_loadu_si256((const __m256 *)(a + len));
        cmp_EOS = _mm256_cmpeq_epi8(vec, EOS);
        clen_EOS = _mm256_movemask_epi8(cmp_EOS);
        cmp_EOC = _mm256_cmpeq_epi8(vec, EOC);
        clen_EOC = _mm256_movemask_epi8(cmp_EOC);
        printf("cmp_EOS(%u, %u, %u) ", clen_EOS, _tzcnt_u32(clen_EOS), _lzcnt_u32(clen_EOS));print256_num(cmp_EOS);
        printf("cmp_EOC(%u, %u, %u) ", clen_EOC, _tzcnt_u32(clen_EOC), _lzcnt_u32(clen_EOC));print256_num(cmp_EOC);
        if (clen_EOC != 0) {
            found = true;
            if (fromend) {
                if (clen_EOS == 0){
                    cdist = len + 32 - _lzcnt_u32(clen_EOC);
                }
                else {
                    uint8_t dist = _tzcnt_u32(clen_EOS);
                    printf("clenEOC bef: %u\n", clen_EOC);
                    dist += 1;
                    clen_EOC = (clen_EOC & ~(0xFFFFFFFF << dist));
                    printf("nand val %u\n", 0xFFFFFFFF << dist);
                    printf("clenEOC aff: %u\n", clen_EOC);
                    return len + (32 - _lzcnt_u32(clen_EOC));
                }
            }
            else{
                cdist = len + _tzcnt_u32(clen_EOC);
                return cdist;
            }
        }
        if (clen_EOS != 0) {
            if (found)
                return cdist;
            return len + _tzcnt_u32(clen_EOS);
        }
        len += 32;
    }
    // ((clen_EOC == 0 && clen_EOS == 0) && (len += 32));

    return 0;
}

