#include <bits/stdc++.h>

using namespace std;

struct input {
    int index;
    int value;
};

bool
compareInput(input i1, input i2)
{
    return (i1.value < i2.value);
}

class Solution {
    public:
        vector<int> two_sum(vector<int>& nums, int target) {
            int left = 0;
            int right = nums.size() - 1;
            vector<int> result;
            vector<input> my_input;
            
            for (int i = 0; i < nums.size(); i++) {
                my_input.push_back(input());
                my_input[i].index = i;
                my_input[i].value = nums[i];
            }
                                                             
            sort(my_input.begin(), my_input.end(), compareInput);
            
            while (left < right) {
                int sum = my_input[left].value + my_input[right].value;
                
                if (sum == target)
                    break;
                else if (sum < target)
                    left++;
                else if (sum > target)
                    right--;
            }
            
            result.push_back(my_input[left].index);
            result.push_back(my_input[right].index);
            
            return result;                                                                                        }
};

int
main(void)
{
    vector<int> input;
    vector<int> output;
    int len = -1;
    int value = -1;
    int target = -1;
    Solution s;

    cout << "Enter the length of the array: ";
    cin >> len;

    cout << "Enter the array elements: " << endl;
    for (int i = 0; i < len; i++) {
        cin >> value;
        input.push_back(value);
    }

    cout << "Enter target value: ";
    cin >> target;

    output = s.two_sum(input, target);

    cout << "Output:" << endl;
    for (int i = 0; i < output.size(); i++) {
        cout << output.at(i) << ' ';
    }
}
