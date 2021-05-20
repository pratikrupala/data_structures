#include <bits/stdc++.h>

using namespace std;

bool negative = false;
bool max_len_crossed = false;
int negative_number = -2147483648;
int positive_number = 2147483647;

bool create_input_stack(string s, stack<int>* input_stack)
{
    bool ret            = false;
    int count           = 0;
    int max_len         = 10;
    int pos             = 0;

    negative = false;
    max_len_crossed = false;
    // Skip leading white spaces, if any.
    while ((s[pos] == ' ') && (pos < s.size()))
        pos++;

    // Check if - or + are present.
    if (pos < s.size()) {
        if (s[pos] == '-') {
            negative = true;
            pos++;
        } else if (s[pos] == '+') {
            pos++;
        }
    }

    // Skip leading zeros, if any.
    while ((s[pos] == '0') && (pos < s.size()))
        pos++;

    // Now traverse the string if there are any char left.
    for (int i = pos; i < s.size(); i++) {
        int temp = 0;
        if (isdigit(s[i])) {
            if (count == max_len) {
                max_len_crossed = true;
                return ret;
            }
            temp = (int)s[i] - 48;
            input_stack->push(temp);
            count++;
        } else {
            break;
        }
    }

    if (count)
        ret = true;

    return ret;
}

class Solution {
    public:
        int myAtoi(string s) {
            stack<int> input_stack;
            int result = 0;
            bool ret = false;
            int power = 0;
            double temp_result = 0;

            ret = create_input_stack(s, &input_stack);
            if (max_len_crossed) {
                if (negative)
                    result = negative_number;
                else
                    result = positive_number;
                max_len_crossed = false;
                goto out;
            }

            if (!ret) {
                result = 0;
                goto out;
            }

            while (!input_stack.empty()) {
                temp_result += input_stack.top() * pow(10, power);
                input_stack.pop();
                power++;
            }

            if (negative) {
                if (temp_result > 2147483648)
                    result = -2147483648;
                else
                    result = -1 * temp_result;
            } else {
                if (temp_result > 2147483647)
                    result = 2147483647;
                else
                    result = temp_result;
            }

out:
            return result;
        }
};

int main()
{
    string input;
    class Solution sol;
    int result = 0;

    cout << "Enter your input: ";
    cin >> input;

    result = sol.myAtoi(input);

    cout << "Result: " << result << endl;

    return 0;
}
