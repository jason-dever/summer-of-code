struct ListNode {
     int val;
     ListNode *next;
     ListNode() : val(0), next(nullptr) {}
     ListNode(int x) : val(x), next(nullptr) {}
     ListNode(int x, ListNode *next) : val(x), next(next) {}
 };

class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        if (!list1)
            return list2;
        if (!list2)
            return list1;

        ListNode* out;
        if (list1->val < list2->val) {
            out = list1;
            list1 = list1->next;

        }
        else {
            out = list2;
            list2 = list2->next;
        }
        ListNode* out_head = out;

        while (true) {
            if (!list1) {
                out->next = list2;
                break;
            }
            if (!list2) {
                out->next = list1;
                break;
            }

            while (list1->val <= list2->val) {
                out->next = list1;
                out = out->next;
                
                list1 = list1->next;
                
                if (!list1) break;
            }
            if (!list1) {
                out->next = list2;
                break;
            }
            while (list2->val <= list1->val) {
                out->next = list2;
                out = out->next;
                
                list2 = list2->next;
                
                if (!list2) break;
            }
        }
        return out_head;
    }
};
