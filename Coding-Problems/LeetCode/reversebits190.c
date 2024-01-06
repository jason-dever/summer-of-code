#include <stdint.h>

uint32_t reverseBits(uint32_t n) {
    uint32_t out = 0;

    for (int i = 0; i <= 31; i++) {
        out |= ((n >> i) & 1) << (31-i);
    }
    return out;
}
