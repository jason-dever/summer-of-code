class Solution:
    def intToRoman(self, num: int) -> str:
        roman_lookups = { 1000 : "M",
                          900 : "CM",
                          500 : "D",
                          400 : "CD",
                          100 : "C",
                          90 : "XC",
                          50 : "L",
                          40 : "XL",
                          10 : "X",
                          9 : "IX",
                          5 : "V",
                          4 : "IV",
                          1 : "I" }

        numeral = ""
        while num:
            for val in roman_lookups:
                symbol = roman_lookups[val]

                while num >= val:
                    num -= val
                    numeral += symbol
                
        return numeral
