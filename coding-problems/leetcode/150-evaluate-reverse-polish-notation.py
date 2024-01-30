from math import trunc

class Solution:
    def evalRPN(self, tokens: list[str]) -> int:
        stack = []
        for token in tokens:
            if token not in "+-/*":
                stack.append(int(token))
            else:
                temp = stack[len(stack)-1]
                stack.pop()
                top = len(stack)-1

                result = 0
                match token:
                    case "+":
                        result = stack[top]+temp
                    case "-":
                        result = stack[top]-temp
                    case "*":
                        result = stack[top]*temp
                    case "/":
                        result = trunc(stack[top]/temp)
                stack[top] = result
                        
        return stack[0]
