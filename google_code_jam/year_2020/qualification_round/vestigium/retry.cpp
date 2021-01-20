#include <iostream>
#include <vector>
#include <set>

using namespace std;

class solution {
    public:
        solution (int total) {
            n = total;
            result = 0;
            row = 0;
            col = 0;
        }
        int calculate_result(void) {
            vector <set<int>> col_set(n);
            set<int> row_set;

            //cout << "Starting the calculation.\n";
            for (int i = 0; i < n; i++) {
                //cout << "i: " << i << endl;
                for (int j = 0; j < n; j++) {
                    int current = 0;
                    cin >> current;

                    if (i == j)
                        result += current;

                    row_set.insert(current);

                    col_set[j].insert(current);
                }
                if (row_set.size() != n)
                    row++;
                row_set.clear();
            }
            //cout << "Midway\n";
            for (int j = 0; j < n; j++) {
                //cout << "j: " << j << endl;
                if (col_set[j].size() != n)
                    col++;
            }
            //cout << "Done" << endl;
            return 0;
        }
        int n;
        int result;
        int row;
        int col;
};

int main() {
    int ret = -1;
    int tc = 0;

    cin >> tc;
    if (tc < 1 || tc > 100) {
        cout << "Invalid no of test cases." << endl;
        goto out;
    }

    for (int i = 0; i < tc; i++) {
        int n = 0;

        cin >> n;
        if (n < 1 || n > 100) {
            cout << "Invalid matix size." << endl;
            goto out;
        }
        solution s(n);
        if (s.calculate_result())
            goto out;
        cout << "Case #" << i + 1 << ": "<< s.result << " " << s.row
            << " " << s.col << endl;
    }
    ret = 0;
out:
    return ret;
}
