class Solution:
    def romanToInt(self, s: str) -> int:
        numerals = { "I": 1, "V": 5, "X": 10, "L": 50, "C": 100, "D": 500, "M": 1000 }
        subtractions = { "IV": 2, "IX": 2, "XL": 20, "XC": 20, "CD": 200, "CM": 200 } 
            
        sum = 0
        for numeral in s:
            sum += numerals[numeral]

        for subtraction in subtractions:
            if subtraction in s:
                sum -= subtractions[subtraction]

        return sum
