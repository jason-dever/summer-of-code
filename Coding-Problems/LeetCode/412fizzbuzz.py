class Solution:
    def fizzBuzz(self, n: int) -> list[str]:
        conditions = { 3 : "Fizz", 5 : "Buzz" }
        out = []

        for i in range(1, n+1):
            element = ""
            default = True

            for condition in conditions:
                if i%condition == 0:
                    element += conditions[condition]
                    default = False

            if default:
                element += str(i)

            out.append(element)

        return out;
