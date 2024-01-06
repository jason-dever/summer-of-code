int numberOfSteps(int num){
    int num_steps = 0;
    
    while (num) {
        num_steps++;

        if (num&1) // Odd
            num--;
        else
            num >>= 1;
    }

    return num_steps;
}
