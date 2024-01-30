class Solution:
    def checkInclusion(self, s1: str, s2: str) -> bool:
        window_size = len(s1)

        for begin in range(len(s2)-len(s1) + 1):
            if self._isPermutation(s1, s2[begin:begin+window_size]):
                return True

        return False

    def _isPermutation(self, s1: str, s2: str) -> bool:
        return sorted(s1) == sorted(s2)
