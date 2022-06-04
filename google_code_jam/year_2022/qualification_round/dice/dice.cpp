#include <iostream>
#include <map>

using namespace std;

int get_count(void) {
	int result = 0;
	int len = 0;
	int input = 0;
	int previous = 0;
	int current_max = 0;
	map<int, int> data;
	map<int, int>::iterator itr;

	cin >> len;

	for (int i = 0; i < len; i++) {
		cin >> input;

		if (data.find(input) == data.end())
			data[input] = 1;
		else
			data[input] += 1;
	}

#if 0
	for (auto x : data) {
		cout << x.first << " -> " << x.second << endl;
	}
#endif

	itr = data.begin();
	if (itr->first < itr->second)
		result += itr->first;
	else
		result += itr->second;
	previous = result;
	itr++;

	for (; itr != data.end(); itr++) {
		int needed = itr->first - previous;

		if (needed > itr->second) {
			previous += itr->second;
			result += itr->second;
		} else {
			previous += needed;
			result += needed;
		}
	}

	return result;
}

int main(void)
{
	int tc = 0;
	int count = -1;

	cin >> tc;

	for (int i = 1; i <= tc; i++) {
		count = get_count();
		cout << "Case #" << i << ": " << count << endl;
	}

	return 0;
}
