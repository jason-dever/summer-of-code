class Solution:
    def characterReplacement(self, s: str, k: int) -> int:
        letter_counts = dict()
        num_most_frequent = 0

        left = 0
        len_longest_substr = 0

        for right, ch in enumerate(s):
            letter_counts[ch] = letter_counts.get(ch, 0)+1

            num_most_frequent = max(num_most_frequent, letter_counts[ch])

            window_size = (right+1 - left)
            
            if window_size-num_most_frequent > k:
                letter_counts[s[left]] -= 1
                left += 1
                continue

            len_longest_substr = max(len_longest_substr, (right+1 - left))
        
        return len_longest_substr
