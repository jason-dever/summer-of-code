int singleNumber(int* nums, int numsSize) {
    int out = 0;

    for (int i = 0; i < numsSize; i++) {
        out ^= nums[i]; 
    }
    return out;
}
