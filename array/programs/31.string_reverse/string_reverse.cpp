#include <bits/stdc++.h>

using namespace std;

void reverse_substring(vector<char>& s, int start, int end)
{
    char tmp;
    while (start <= end) {
        tmp = s[start];
        s[start] = s[end];
        s[end] = tmp;
        start++;
        end--;
    }
}

class Solution {
    public:
        void reverseWords(vector<char>& s) {
            int start = 0;
            int end = -1;

            if (s.size() == 1) {
                cout << s[0] << endl;
                return;
            }

            for (int i = 0; i < s.size(); i++) {
                if ((s[i] == ' ' || i == s.size() - 1) && start >= 0) {
                    if (s[i] == ' ')
                        end = i - 1;
                    else if (i == s.size() - 1)
                        end = i;
                    reverse_substring(s, start, end);
                    start = i + 1;
                    end = -1;
                }
            }

            reverse_substring(s, 0, s.size() - 1);
            for (int i = 0; i < s.size(); i++) {
                cout << s[i];
            }
            cout << endl;
        }
};

int main(void)
{
    int ret = -1;
    string s;
    Solution my_solution;

    cout << "Enter the input string: ";
    getline(cin, s);
    vector<char> input(s.begin(), s.end());

    my_solution.reverseWords(input);

#if 0
    for (int i = 0; i < input.size(); i++) {
        cout << input[i];
    }
    cout << endl;
#endif

    return ret;
}
