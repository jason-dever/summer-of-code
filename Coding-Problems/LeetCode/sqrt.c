int mySqrt(int x){
    int sub = 1;
    int count = 0;

    while (x > 0) {
        x -= sub;
        sub += 2;
        count += 1;
    }
    if (x < 0) {
        count--;
    }

    return count;
}
