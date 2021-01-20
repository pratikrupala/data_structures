#include <iostream>

using namespace std;

int
read_char(string str, int pos)
{
    char ch;

    ch = str.at(pos);

    return ((int) ch - '0');
}

void
generate_nested_depth_str(int tc)
{
    int pos = 0;
    int num = 0;
    int temp_num = 0;
    string input_str;
    string output_str;

    cin >> input_str;
    int len = input_str.length();

    num = read_char(input_str, pos);
    temp_num = num;

    while (temp_num) {
        output_str.push_back('(');
        temp_num--;
    }
    
    output_str.push_back(input_str.at(pos));
    pos++;

    while (pos < len) {
        int next_num = 0;
        
        next_num = read_char(input_str, pos);

        /* Below while loop will take care of repeatitive numbers. */
        while (next_num == num) {
            output_str.push_back(input_str.at(pos));
            pos++;
            if (pos < len)
                next_num = read_char(input_str, pos);
            else
                break;
        }

        if (pos < len) {
            if (next_num > num) {
                for (int k = 0; k < (next_num - num); k++) {
                    output_str.push_back('(');
                }
            } else {
                for (int k = 0; k < (num - next_num); k++) {
                    output_str.push_back(')');
                }
            }
            output_str.push_back(input_str.at(pos));
            pos++;
        }

        num = next_num;
    }

    while (num) {
        output_str.push_back(')');
        num--;
    }

    cout << "Case #" << tc << ": " << output_str << endl;
}

int
main(void)
{
    int tc = -1;

    cin >> tc;

    for (int i = 0; i < tc; i++) {
        generate_nested_depth_str(i + 1);
    }
}
