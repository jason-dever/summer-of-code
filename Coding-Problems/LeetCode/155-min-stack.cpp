#include <algorithm>
#include <limits>

class StackNode {
public:
    int val;
    int min = std::numeric_limits<int>::max();
    StackNode* next = nullptr;
};

class MinStack {
private:
    StackNode* head;
public:
    MinStack() {
        head = new StackNode;
    }
    
    ~MinStack() {
        while (head->next != nullptr) {
            this->pop();
        }
        delete head;
    }

    void push(int val) {
        StackNode* temp = new StackNode;
        temp->val = head->val;
        temp->min = head->min;
        temp->next = head->next;

        head->min = std::min(temp->min, val);
        head->val = val;
        head->next = temp;
    }

    void pop() {
        StackNode* temp = head->next;
        delete head;
        head = temp;
    }

    int top() {
        return head->val;
    }

    int getMin() {
        return head->min;
    }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */
