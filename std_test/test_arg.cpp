#include "ArgOpt.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string teststr = "0,0,0,0,0.109506,-0.0680305,0.0596426,0.00317401,-0.00143468,-0.0753419,-0.190461,-0.211144,0.171134,0.0471445,0.00317401,0.00143468,0.0753419,-0.190461,0.211144,0.171134,-0.0471445,0,0,0,0,0,0,0,0,0,0,0,0,0,0";

int str_sep(const std::string &src_str, const char sep, std::vector<std::string> &res)
{
    std::stringstream ss(src_str);
    std::string item;
    while (getline(ss, item, sep))
    {
        //std::cout << "item=" << item << std::endl;
        res.push_back(item);
    }
    return (int)res.size();
}

int main(int argc, char *argv[])
{
    std::vector<std::string> frame_data;
    int data_num = str_sep(teststr, ',', frame_data);
    if (data_num > 6)
    {
        float hpos = std::stof(frame_data[4]);
        float vpos = std::stof(frame_data[5]);
        printf("get data:%d %f %f\n", data_num, hpos, vpos);
    }
    else
    {
        printf("error, no enought data\n");
    }
    return 0;
}

void test_argopt()
{
    liblz::ArgOpt opt;

    std::vector<std::string> args;
    args.push_back("mycmd");
    args.push_back("mycmd2");
    args.push_back("mycmd3");

    args.push_back("-t");
    args.push_back("10");

    args.push_back("-n");
    args.push_back("9");
    args.push_back("pp");

    args.push_back("-s");

    args.push_back("-h");
    args.push_back("01");
    args.push_back("00");
    args.push_back("00");
    args.push_back("00");

    opt.parse_options(args);
    opt.show_options();
}
