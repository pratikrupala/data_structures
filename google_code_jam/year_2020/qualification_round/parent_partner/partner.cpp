#include <iostream>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

struct time_entry {
    int mins;
    int start;  /* If start is 1, then it is start time else end time. */
    int group;

    time_entry (int mins, int start, int group)
        : mins(mins), start(start), group(group)
    {
    }
};

struct compare_time {
    bool operator()(time_entry const& t1, time_entry const& t2)
    {
        /**
         * If times are matching then two possibilities:
         * [1] The two entries have same start and end times.
         * [2] End time of previous entry is matching start time of next entry.
         *
         * For [1], no need to differentiate the order. Any partner can take
         * the task.
         * For [2], let end time take the precedence. So if t->start is 1 then
         * it is start time. And hence, (t1->start < t2->start) will decide
         * the precedence by end time (with t->start being 0).
         */
        return t1.mins == t2.mins
            ? t1.start > t2.start
            : t1.mins > t2.mins;
    }
};

class solution {
    public:
        solution(int total) {
            n = total;
            partner_map.insert(pair<char, int>('C', 0));
            partner_map.insert(pair<char, int>('J', 0));
        }
        void take_input(void) {

            for (int i = 0; i < n; i++) {
                int first = 0;
                int second = 0;

                cin >> first;
                cin >> second;

                input_queue.push(time_entry(first, 1, i + 1));
                input_queue.push(time_entry(second, 0, i + 1));
            }

            /*
            while (!input_queue.empty()) {
                time_entry t = input_queue.top();
                input_queue.pop();
                cout << "Group " << t.group << ":" << endl;
                cout << "Time : " << t.mins << " start : " << t.start << endl;
                cout << endl;
            }
            */
        }

        int release_partner(int group) {
            map<char, int>::iterator itr;
            int ret;
            bool found = false;

            for (itr = partner_map.begin(); itr != partner_map.end(); ++itr) {
                if (itr->second == group) {
                    itr->second = 0;
                    return 0;
                }
            }

            return -1;
        }

        char assign_partner(int group) {
            map<char, int>::iterator itr;
            char ret;
            bool found = false;

            for (itr = partner_map.begin(); itr != partner_map.end(); ++itr) {
                if (itr->second == 0) {
                    itr->second = group;
                    ret = itr->first;
                    found = true;
                    break;
                }
            }

            if (found)
                return ret;
            else
                return 'I';
        }

        int generate_result(void) {
            map<int, char>::iterator itr;
            while (!input_queue.empty()) {
                char tmp;
                int ret;
                time_entry t = input_queue.top();
                input_queue.pop();
#if 0
                cout << "Group " << t.group << ":" << endl;
                cout << "Time : " << t.mins << " start : " << t.start << endl;
                cout << endl;
#endif

                if (t.start) {
                    tmp = assign_partner(t.group);
                    if (tmp == 'I') {
                        result.clear();
                        result = "IMPOSSIBLE";
                        return 0;
                    } else {
                        result_map.insert(pair<int, char>(t.group, tmp));
                    }
                } else {
                    if (release_partner(t.group))
                        return -1;
                }
            }

            for (itr = result_map.begin(); itr != result_map.end(); ++itr) {
                result.push_back(itr->second);
            }
            return 0;
        }

        void print_result(void) {
            for (int i = 0; i < result.size(); i++) {
                cout << result.at(i);
            }
            cout << endl;
        }

        int n;
        string result;
        priority_queue<time_entry, vector<time_entry>, compare_time> input_queue;
        map<char, int> partner_map;
        map<int, char> result_map;
};

int main(void)
{
    int ret = -1;
    int tc = 0;

    cin >> tc;

    for (int i = 0; i < tc; i++) {
        int total = 0;

        /* Take the total entries as input. */
        cin >> total;

        solution s(total);
        s.take_input();
        ret = s.generate_result();
        if (ret) {
            cout << "Case #" << i + 1 << ": IMPOSSIBLE" << endl;
        } else {
            cout << "Case #" << i + 1 << ": ";
            cout << s.result << endl;
        }
    }
    ret = 0;
out:
    return ret;
}
