#include <immintrin.h>
#include <stdint.h>
#include <stdlib.h>

void _avx_cpy(void *d, const void *s, size_t n) {  
    // d, s -> 32 byte aligned  
    // n -> multiple of 32  
    auto *dVec = reinterpret_cast<__m256i *>(d);  
    const auto *sVec = reinterpret_cast<const __m256i *>(s);  
    size_t nVec = n / sizeof(__m256i);  
    for (; nVec > 0; nVec--, sVec++, dVec++) {    
        const __m256i temp = _mm256_load_si256(sVec);    
        _mm256_store_si256(dVec, temp);  
    }
}