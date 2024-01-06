def isPalindrome(s:str) -> bool:
    whitelist = "abcdefghijklmnopqrstuvwxyz0123456789"

    alphanumeric_str = ""

    for char in s.lower():
        if char in whitelist:
            alphanumeric_str += char

    return alphanumeric_str == alphanumeric_str[::-1]
