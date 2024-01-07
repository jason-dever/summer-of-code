int maximumWealth(int** accounts, int accountsSize, int* accountsColSize) {
    int highest_sum = 0;

    for (int i = 0; i < accountsSize; i++) {
        int current_sum = 0;
        
        for (int j = 0; j < *accountsColSize; j++) {
            current_sum += accounts[i][j];       
        }
        if (current_sum > highest_sum)
            highest_sum = current_sum;
    }
    return highest_sum;
}
