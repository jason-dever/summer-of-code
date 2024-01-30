#include <stdint.h>
#include <immintrin.h>

int hammingWeight(uint32_t n) {
    return _mm_popcnt_u32(n);
}
