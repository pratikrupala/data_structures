#include <iostream>
#include <vector>
#include <set>

using namespace std;

class solution {
    public:
        solution(int total) {
            n = total;
            row = 0;
            col = 0;
            result = 0;
        }
        int take_input(void) {
            set<int> exclude_row;
            set<int> exclude_col;
            vector <set<int>> row_set(n);
            vector <set<int>> col_set(n);

            arr = (int **) malloc(n * sizeof(*arr));
            if (!arr) {
                cout << "Failed to allocate memory1." << endl;
                return -1;
            }

            for (int i = 0; i < n; i++) {
                arr[i] = (int *) malloc(n * sizeof(int));
                if (!(arr[i])) {
                    cout << "Failed to allocate memory2." << endl;
                    return -1;
                }
            }

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    cin >> arr[i][j];

                    if (i == j) {
                        result += arr[i][j];
                    }

                    if ((exclude_row.find(i) == exclude_row.end()) && (row_set[i].find(arr[i][j]) != row_set[i].end())) {
                        row++;
                        exclude_row.insert(i);
                    } else {
                        row_set[i].insert(arr[i][j]);
                    }

                    if ((exclude_col.find(j) == exclude_col.end()) && (col_set[j].find(arr[i][j]) != col_set[j].end())) {
                        col++;
                        exclude_col.insert(j);
                    } else {
                        col_set[j].insert(arr[i][j]);
                    }
                }
            }

            /*
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    cout << arr[i][j];
                    cout << "\t";
                }
                cout << endl;
            }
            */
            return 0;
        }
        int n;
        int **arr = NULL;
        int row;
        int col;
        int result;
};

int main(void)
{
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
        if (s.take_input())
            goto out;
        cout << "Case #" << i + 1 << ": "<< s.result << " " << s.row
            << " " << s.col << endl;
    }
    ret = 0;
out:
    return ret;
}
