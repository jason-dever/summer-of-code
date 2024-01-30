class NumMatrix:
    def __init__(self, matrix: list[list[int]]):
        self._prefix_sum_array = [[0]*(len(matrix[0])+1) for a in range(len(matrix)+1)]

        for row in range(len(self._prefix_sum_array)-1):
            for col in range(len(self._prefix_sum_array[0])-1):
                self._prefix_sum_array[row+1][col+1] = matrix[row][col] + self._prefix_sum_array[row][col+1] + self._prefix_sum_array[row+1][col] - self._prefix_sum_array[row][col] 

        

    def sumRegion(self, row1: int, col1: int, row2: int, col2: int) -> int:
        return self._prefix_sum_array[row2+1][col2+1] - self._prefix_sum_array[row1][col2+1] - self._prefix_sum_array[row2+1][col1] + self._prefix_sum_array[row1][col1]

# Your NumMatrix object will be instantiated and called as such:
# obj = NumMatrix(matrix)
# param_1 = obj.sumRegion(row1,col1,row2,col2)
