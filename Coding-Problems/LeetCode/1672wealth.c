int maximumWealth(int** accounts, int accountsSize, int* accountsColSize) {
    int highest_sum = 0;
    int current_sum = 0;

    for (int i = 0; i < accountsSize; i++) {
        for (int j = 0; j < *accountsColSize; j++) {
            current_sum += accounts[i][j];       
        }
        if (current_sum > highest_sum)
            highest_sum = current_sum;

        current_sum = 0;
    }
    return highest_sum;
}
