#include <cstdlib>
#include <cstring>
#include <iostream>

using std::cin;
using std::cout;

int mul(int a, int b)
{
    return a * b;
}

int power(int base, int exponent)
{
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

int charToInt(const char* char_input)
{
    const int ASCII_NEGATIVE = 45;
    const int ASCII_ZERO = 48;
    size_t len = strlen(char_input);
    int result = 0;

    if (char_input[0] == ASCII_NEGATIVE) {
        for (int i = 1; i < len; i++) {
            if ((char_input[i] > 57) || (char_input[i] < 48)) {
                cout << "You can't input sth that is not a nunber.";
                exit(0);
            }
            result += power(10, (len - 1) - i) * (char_input[i] - ASCII_ZERO);
        }
        result = -result;
    } else {
        for (int i = 0; i < len; i++) {
            if ((char_input[i] > 57) || (char_input[i] < 48)) {
                cout << "You can't input sth that is not a nunber.";
                exit(1);
            }
            result += power(10, (len - 1) - i) * (char_input[i] - ASCII_ZERO);
        }
    }

    return result;
}

int* input_command(const int argc, char** argv)
{
    int* input = new int[argc - 1] {};
    for (int i = 1; i < argc; i++) {
        char* commandInput = *(argv + i);
        input[i - 1] = charToInt(commandInput);
    }
    return input;
}
int* input_cin()
{
    char* input_line = new char[64] {};
    cout << "Please input two integers(using one space to separate): " << '\n';
    cin.get(input_line, 64);
    int* input = new int[2] {};
    int index = 0;

    for (int i = 0; i < 2; i++) {
        char* input_tem_char = new char[32] {};

        for (int j = 0; j < strlen(input_line); j++) {
            if (i == 0) {
                if (input_line[j] == ' ') {
                    while (input_line[j] == ' ') {
                        index = j;
                        j++;
                    }
                    break;
                }
                input_tem_char[j] = input_line[j];
            } else {
                input_tem_char[j] = input_line[j + index + 1];
                if (j + index + 1 >= strlen(input_line)) {
                    break;
                }
            }
        }
        input[i] = charToInt(input_tem_char);
        delete[] input_tem_char;
    }

    delete[] input_line;
    return input;
}

int main(int argc, char** argv)
{
    if (argc > 1) {
        int* input_values = input_command(argc, argv);
        int result = mul(input_values[0], input_values[1]);
        cout << input_values[0] << " * " << input_values[1] << " = " << result << '\n';
        delete[] input_values;
    } else {
        int* input_values = input_cin();
        int result = mul(input_values[0], input_values[1]);
        cout << input_values[0] << " * " << input_values[1] << " = " << result << '\n';
        delete[] input_values;
    }
    return 0;
}
