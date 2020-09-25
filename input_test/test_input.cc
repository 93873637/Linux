
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int str_sep(const std::string src_str, std::vector<std::string> &res)
{
    std::string str_item;
    stringstream input(src_str);
    while (input >> str_item)
    {
        res.push_back(str_item);
    }
    return 0;
}

#define CMD_PROMPT ">> "
#define TRACE_INFO printf

void test_input2()
{
    TRACE_INFO("set mode to homing ok\n");
    TRACE_INFO("Press 'y' when ready, 'q' to exit...\n");
    while (1)
    {
        char c = getchar();
        if (c == 'q')
            return;
        if (c == 'y')
            break;
    }

    // // 3. get current/max/min pos
    // TRACE_INFO("SCA current/max/min pos:\n");
    // SCA_GET_DATA_FLOAT(Position_Real);
    // SCA_GET_DATA_FLOAT(Position_Limit_H);
    // SCA_GET_DATA_FLOAT(Position_Limit_L);

    // 4. get max/min value from user
    TRACE_INFO("Please input position limit high: ");
    float high;
    cin >> high;
    TRACE_INFO("Please input position limit low: ");
    float low;
    cin >> low;

    TRACE_INFO("Now set current pos(%f) as home value with limit (%f, %f), confirm?(y/n)\n", 1.2, low, high);
    while (1)
    {
        char c = getchar();
        if (c == 'q' || c == 'n')
            return;
        if (c == 'y')
            break;
    }

    std::cout << "set high/low: " << high << ", " << low << std::endl;
}

void test_input()
{
    std::string str_input;

    std::cout << "Please, enter your full name: ";
    std::getline(std::cin, str_input);
    std::cout << "Hello, " << str_input << "!\n";

    // std::string str_input = "";
    // do
    // {
    //     cout << CMD_PROMPT;
    //     std::string total = "";
    //     while(cin >> str_input) {
    //         total += str_input;

    //     };
    //     if (str_input == "q" || str_input == "Q")
    //     {
    //         break;
    //     }
    //     else
    //     {
    //         cout << "\"" << total << "\"" << endl;
    //     }
    // } while (true);

    //用于存放分割后的字符串
    std::vector<string> res;
    //str_sep(" this is my lean C++ !!!", res);
    str_sep(str_input, res);

    // //待分割的字符串，含有很多空格
    // string word="   Hello, I want   to learn C++!   ";
    // //暂存从word中读取的字符串
    // string result;
    // //将字符串读到input中
    // stringstream input(word);
    // //依次输出到result中，并存入res中
    // while(input>>result)
    //     res.push_back(result);
    //输出res
    for (int i = 0; i < res.size(); i++)
    {
        cout << "res[" << i << "] = " << res[i] << endl;
    }

    // istringstream str(" this is a   text");
    // string out;

    // while (str >> out) {
    // 	cout << out << endl;
    // }
}

// #include <iostream>
// #include <string>
// using namespace std;
// #define CMD_SETPOS    "setpos>> "
// int main()
// {
//     std::string str_prompt = ">> ";
//     std::string str_input = "";
//     do
//     {
//         cout << str_prompt;
//         cin >> str_input;
//         if (str_input == "q" || str_input == "Q")
//         {
//             break;
//         }
//         //cout << str_input << endl;
//         if (str_prompt == CMD_SETPOS)
//         {
//             float val = std::stof(str_input);
//             cout << "get val: " << val << endl;
//         }
//         else
//         {
//             if (str_input == "setpos")
//             {
//                 str_prompt = CMD_SETPOS;
//             }
//             else
//             {
//                 cout << "unknonw command: " << str_input << endl;
//             }
//         }
//     } while (true);//std::string result_str, str;
// cout << "Enter strings" << endl;
// while (cin >> str && str[str.length() - 1] != '\n')
// {
//     cout << str;
//     result_str = result_str + str;
// }
// cout << result_str << endl;
//     return 0;
// }

int main(int argc, char **argv)
{
    test_input2();
    return 0;
}
