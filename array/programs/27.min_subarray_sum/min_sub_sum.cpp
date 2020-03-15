#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class Solution {
	public:
		int minSubArrayLen(int s) {
			if (nums.size() == 0)
				return 0;
			if (nums.size() == 1) {
				if (nums[0] == s)
					return 1;
				else
					return 0;
			}
			int found = 0;
			int front = 0;
			int rear = 0;
			int len = nums.size();
			int curr_sum = nums[0];
			int min_len = INT_MAX;

			while ((rear < len) && (front < len)) {
				if (curr_sum < s) {
					if (++rear < len) {
						curr_sum += nums[rear];
					}
				} else {
					if (min_len > (rear - front + 1)) {
						min_len = (rear - front + 1);
						found = 1;
					}
					if (++front < len) {
						curr_sum -= nums[front - 1];
					}
				}
			}
			cout << "Found: " << found << endl;
			if (found == 0)
				min_len = 0;
			return min_len;
		}

		void take_input(void) {
			cout << "Enter the length of the input array: ";
			cin >> this->vec_len;

			//cout << "Your entered length: " << this->vec_len << endl;

			cout << "Enter the input numbers: ";
			for (int i = 0; i < this->vec_len; i++) {
				int tmp;
				cin >> tmp;
				this->nums.push_back(tmp);
			}

			/*
			   for (int i = 0; i < this->vec_len; i++) {
			   cout << this->nums[i];
			   cout << " ";
			   }
			   cout << endl;
			   */
		}
		vector<int> nums;
		int vec_len;
};

int main(void)
{
	int target = 0;
	int result = 0;
	Solution S1;

	S1.take_input();

	cout << "Enter target number: ";
	cin >> target;

	result = S1.minSubArrayLen(target);
	cout << "Result: " << result << endl;
};
