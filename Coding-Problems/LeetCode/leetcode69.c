#include <stdio.h>

int mySqrt(int x) {
    int count = 0;
    
    for (int sub = 1; x > 0; sub += 2) {
        x -= sub;
        count++;
    } 
    if (x < 0) {
        count--;
    }
    return count;
}

int main() {
    printf("%d\n", mySqrt(4));
    printf("%d\n", mySqrt(8));
    printf("%d\n", mySqrt(16));
    return 0;
}
