#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;

void chain_reaction(void)
{
	int len = 0;
	long long final_result = 0;

	cin >> len;

	int fun[len];
	int pointer[len];

	vector<vector<pair<int,int>>> data;
	vector<pair<int, int>> data_itr;

	map<int, int> ready_to_process;
	map<int, int>::iterator map_itr;

	map<int, int> total_entry;
	map<int, int>::iterator entry_itr;

	set<int> initiators;
	set<int>::iterator initiators_itr;

	/* Fetch all the fun factors of each node */
	for (int i = 0; i < len; i++) {
		cin >> fun[i];
	}

	/* Fetch the node number for each node to which current node points */
	for (int i = 0; i < len; i++) {
		cin >> pointer[i];
	}

	/* Node numbers are sequencial. Just initialize the set sequencially */
	for (int i = 1; i <= len; i++) {
		initiators.insert(i);
	}

	/* Initialize the 2D matrix so that we can later reference any element of it */
	for (int i = 0; i < len; i++) {
		vector<pair<int,int>> my_vec;
		data.push_back(my_vec);
	}

	for (int i = 0; i < len; i++) {
		if (!pointer[i])
			continue;

		/*
		 * Populate data[i] with all the pointers pointing to it.
		 * Also store the fun factor offered by its pointer. But
		 * initially, fun factor offer will be zero which will be
		 * later populated.
		 */
		data[pointer[i] - 1].push_back(make_pair(i + 1, -1));

		/*
		 * For the nodes, which are being pointed by some other
		 * node/s, no need to keep them in intiators' list.
		 */
		initiators.erase(pointer[i]);

		/*
		 * Store the numbers of pointers pointing to each node.
		 */
		map_itr = ready_to_process.find(pointer[i] - 1);
		entry_itr = total_entry.find(pointer[i] - 1);
		if (map_itr != ready_to_process.end()) {
			map_itr->second += 1;
			entry_itr->second += 1;
		} else {
			ready_to_process.insert(pair<int, int> (pointer[i] - 1, 1));
			total_entry.insert(pair<int, int> (pointer[i] - 1, 1));
		}

	}

#if 0
	for (int i = 0; i < len; i++) {
		data_itr = data[i];
		//cout << "Index: ";
		//for (auto x : data_itr) {
		for (auto x : data_itr) {
			cout << "(" << x.first << ", " << x.second << "), ";
		}
		cout << endl;
	}

	for (map_itr = ready_to_process.begin(); map_itr != ready_to_process.end(); map_itr++) {
		cout << "Map" << endl;
		cout << map_itr->first + 1 << " : " << map_itr->second << endl;
	}

	cout << "Initiators: " << endl;
	for (initiators_itr = initiators.begin(); initiators_itr != initiators.end(); initiators_itr++) {
		cout << *initiators_itr << endl;
	}
#endif

	long long max = 0;
	/*
	 * Iterate through each iterator and keep going up, until we can't move up.
	 * Reason for not able ot move up would be that there are other paths pointing
	 * to same upper node which are yet to be processed.
	 * So, upper node will be later processes when its all paths are processed.
	 */
	for (initiators_itr = initiators.begin(); initiators_itr != initiators.end(); initiators_itr++) {
		int current_node = *initiators_itr;
		int target = pointer[*initiators_itr - 1];
		max = fun[*initiators_itr - 1];
back:
		if (target == 0) {
			/*
			 * This node is pointing to abyss. So add it to final result.
			 * But keep processing as there could be more path leading to
			 * the abyss.
			 */
			final_result += max;
			continue;
		}

		int total_entries = total_entry[target - 1];

		/*
		 * Process all the nodes which are pointing to this node.
		 */
		for (int i = 0; i <= total_entries; i++) {
			if (data[target - 1][i].first == current_node) {
				data[target - 1][i].second = max;
				ready_to_process[target - 1] -= 1;
				break;
			}
		}
		
		/*
		 * If all the paths coming to the current node are processed, then we can
		 * process the current node as well.
		 */
		if (!ready_to_process[target - 1]) {
			int current_value = fun[target - 1];
			long long temp_total = 0;
			data_itr = data[target - 1];
			int min = data_itr[0].second;

			/*
			 * First add fun factors of all the nodes coming to the target,
			 * later we will skip the smallest fun factor as it will be
			 * overridden by current node.
			 */
			for (int i = 0; i < total_entries; i++) {
				if (data_itr[i].second < min)
					min = data_itr[i].second;
				else if (data_itr[i].second > max)
					max = data_itr[i].second;
				temp_total += data_itr[i].second;
			}
			temp_total -= min;

			/*
			 * Now is the time to comaring the current node's value with
			 * min value.
			 */
			final_result += temp_total;
			if (current_value > min) {
				max = current_value;
			} else {
				max = min;
			}

			current_node = target;
			target = pointer[target - 1];
			if (target <= 0) {
				final_result += max;
				continue;
			} else {
				goto back;
			}
		} else {
			continue;
		}
	}
	cout << final_result << endl;
}

int main(void)
{
	int tc = 0;

	cin >> tc;

	for (int i = 0; i < tc; i++) {
		cout << "case #" << i + 1 << ": ";

		chain_reaction();
	}
}
