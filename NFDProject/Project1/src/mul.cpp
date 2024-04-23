#include <cstdio>
#include <iostream>
#include <regex>
#include <string>

using namespace std;

regex input_std("(\\+?|-)\\d+");

bool check_valid(string input_str)
{
    bool isValid = false;
    if (regex_match(input_str, input_std)) {
        isValid = true;
    }
    return isValid;
}

void clear_io()
{
    while (getchar() != '\n')
        ;
}

void get_input(string& mul1, string& mul2)
{
    cout << "Please input two integers: " << '\n';
    cin >> mul1 >> mul2;

    bool isValid = false;
    while (!isValid) {
        char next_char = 0;
        while ((next_char = getchar()) != '\n') {
            if (next_char != ' ') {
                clear_io();
                cout << "More than two integers, Please input two integers: " << '\n';
                cin >> mul1 >> mul2;
            }
        }
        if (check_valid(mul1) && check_valid(mul2)) {
            isValid = true;
        } else {
            cout << "Wrong input, Please input two integers: " << '\n';
            cin >> mul1 >> mul2;
        }
    }
}

int get_sign(string& number)
{
    int sign = 1;
    if (number[0] == '-') {
        sign = -1;
        number = &number[1];
    } else {
        if (number[0] == '+') {
            number = &number[1];
        }
    }
    return sign;
}

string mul(string& mul1, string& mul2)
{
    int sign1 = get_sign(mul1);
    int sign2 = get_sign(mul2);
    char sign = ((sign1 * sign2) == 1) ? 0 : '-';
    int len1 = mul1.length();
    int len2 = mul2.length();
    int length = len1 + len2;

    int* temp_values = new int[length] { 0 };

    for (int i = len2 - 1; i >= 0; i--) {
        for (int j = len1 - 1; j >= 0; j--) {
            temp_values[length - 2 - (i + j)] += (mul1[j] - '0') * (mul2[i] - '0');
        }
    }
    for (int i = 0; i < length; i++) {
        temp_values[i + 1] += temp_values[i] / 10;
        temp_values[i] %= 10;
    }

    string result;
    result.push_back(sign);
    for (int i = length - 1; i >= 0; i--) {
        result.push_back(temp_values[i] + '0');
    }

    delete[] temp_values;
    return result;
}

int main(int argc, char const* argv[])
{
    string mul1;
    string mul2;
    if (argc == 3) {
        mul1 = argv[1];
        mul2 = argv[2];
    } else {
        if (argc != 1) {
            cout << "Wrong numbers of arguments!" << '\n';
            get_input(mul1, mul2);
        } else {
            get_input(mul1, mul2);
        }
    }

    cout << mul1 << " * " << mul2 << " = " << mul(mul1, mul2) << '\n';

    return 0;
}