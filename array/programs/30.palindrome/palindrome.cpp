#include <bits/stdc++.h>

using namespace std;

class Solution {
    public:
        bool isPalindrome(string s) {
            int left = 0;
            int diff = 0;
            bool failed = false;
            int right = s.size() - 1;
            bool result = false;

            while (left <= right) {
                if (!isalnum(s[left])) {
                    left++;
                    continue;
                }

                if (!isalnum(s[right])) {
                    right--;
                    continue;
                }

                diff = (int) s[left] - (int) s[right];
                cout << endl;
                cout << "Left: " << (int)s[left] << ", Right: " << (int)s[right] << endl;
                cout << endl;

                if (isdigit(s[left]) || isdigit(s[right])) {
                    if (diff == 0) {
                        left++;
                        right--;
                    } else {
                        failed = true;
                        break;
                    }
                } else {
                    if (diff == 0 || diff == 32 || diff == -32) {
                        left++;
                        right--;
                    } else {
                        failed = true;
                        break;
                    }
                }
            }

            if (!failed)
                result = true;

            return result;
        }
};

int main(void)
{
    int ret = -1;
    bool result = false;
    Solution s;
    string input;

    cout << "Enter the input string: ";
    getline(cin, input);

    cout << endl << "Input: " << input << endl;
    result = s.isPalindrome(input);
    if (result)
        cout << "It is a palindrome string." << endl;
    else
        cout << "It is not a palindrome string." << endl;

    return ret;
}
