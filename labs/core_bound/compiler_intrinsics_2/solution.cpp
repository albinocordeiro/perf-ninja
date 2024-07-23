#include "solution.hpp"

#include <cstring>

#ifdef __x86_64__
#include <immintrin.h>
#include <xmmintrin.h>
#endif

#include <bitset>
#include <cstdint>
#include <iostream>

unsigned solution(const std::string &inputContents) {

    const char *p = inputContents.c_str();
    const int n = inputContents.length();
    int i = 0;


    unsigned len = 0;
    unsigned ans = 0;
    for (; i + 63 < n;) {

#ifdef __x86_64__
        __m256i v1 = _mm256_loadu_si256((__m256i *) &p[i]);
        __m256i v2 = _mm256_loadu_si256((__m256i *) &p[i + 32]);
        __m256i n1 = _mm256_cmpeq_epi8(v1, _mm256_set1_epi8('\n'));
        __m256i n2 = _mm256_cmpeq_epi8(v2, _mm256_set1_epi8('\n'));
        uint64_t m1 = _mm256_movemask_epi8(n1);
        uint64_t m2 = _mm256_movemask_epi8(n2);
        uint64_t msk = m1 | (m2 << 32);

#else
        char vals[64];
        memcpy(vals, i + p, 64);

        uint64_t msk = 0;
        for (int j = 0; j < 64; ++j)
            msk |= uint64_t(vals[j] == '\n') << j;
#endif
        if (!msk) {
            len += 64;
            i += 64;
        } else {
            unsigned leading_zeroes = __builtin_ctzll(msk);
            len = leading_zeroes ? len : 0;
            len += leading_zeroes;
            ans = std::max(ans, len);
            uint64_t new_msk = msk >> 1 + leading_zeroes;
            if (new_msk) {
                unsigned new_leading_zeroes = __builtin_ctzll(new_msk);
                ans = std::max(ans, new_leading_zeroes);
                len = 0;
                i += leading_zeroes + new_leading_zeroes + 2;
            } else {
                unsigned new_leading_zeroes = 64 - (leading_zeroes + 1);
                len = new_leading_zeroes;
                i += 64;
            }
        }
    }
    for (; i < n; ++i) {
        len++;
        len &= -(p[i] != '\n');
        ans = std::max(ans, len);
    }


    return ans;
}
