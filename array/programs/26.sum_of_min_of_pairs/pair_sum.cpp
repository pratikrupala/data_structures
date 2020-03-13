#include <iostream>
#include <vector>

using namespace std;

class Solution {
	public:
		vector<int> twoSum(int target) {
			this->indices.push_back(0);
			this->indices.push_back(this->numbers.size() - 1);
			int *start = this->indices.data();
			int *end = start + 1;

			while (*start < *end) {
				int tmp = this->numbers[*start] + this->numbers[*end];
				if (tmp == target) {
					*start += 1;
					*end += 1;
					break;
				} else if (tmp < target) {
					*start += 1;
				} else {
					*end -= 1;
				}
			}
			return this->indices;
		}

		void take_input(void) {
			cout << "Enter the length of the input array: ";
			cin >> this->vec_len;

			//cout << "Your entered length: " << this->vec_len << endl;

			cout << "Enter the input numbers: ";
			for (int i = 0; i < this->vec_len; i++) {
				int tmp;
				cin >> tmp;
				this->numbers.push_back(tmp);
			}

			/*
			for (int i = 0; i < this->vec_len; i++) {
				cout << this->numbers[i];
				cout << " ";
			}
			cout << endl;
			*/
		}
		vector<int> numbers;
		vector<int> indices;
		int vec_len;
};

int main(void)
{
	int target = 0;
	Solution S1;

	S1.take_input();

	cout << "Enter the target number: ";
	cin >> target;
	S1.twoSum(target);

	cout << "Resultant indices are " << S1.indices[0] << " and " << S1.indices[1] << endl;
	return 0;
}
