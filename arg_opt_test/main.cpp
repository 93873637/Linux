
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ArgOpt.h"

//////////////////////////////////////////////////////////////////////////////////////
// Test Func Micros

#define LOG_PRINT(_fmt, _arg...) printf("%s:%d/%s - " _fmt, __FILE__, __LINE__, __FUNCTION__, ##_arg)

#define TEST_FUNC_E printf("\n%s:%d/%s - E...\n", __FILE__, __LINE__, __FUNCTION__)
#define TEST_FUNC_X printf("%s:%d/%s - X.\n", __FILE__, __LINE__, __FUNCTION__)

#define __TEST_FUNC_BEGIN__(_func_name) \
    void _func_name()                   \
    {                                   \
        TEST_FUNC_E;                    \
        printf("-------------------------------------------\n");

#define __TEST_FUNC_END__                                    \
    printf("-------------------------------------------\n"); \
    TEST_FUNC_X;                                             \
    }

// Test Func Micros
//////////////////////////////////////////////////////////////////////////////////////

__TEST_FUNC_BEGIN__(test2)
{
    liblz::CArgOpt opt("head1 head2 head3 he -t 10 -a 99 -s aaa -b 5.0 -hex 00 99 08 35 -j 8");
    opt.show_options();
}
__TEST_FUNC_END__

__TEST_FUNC_BEGIN__(test_1)
{
    std::vector<std::string> args;
    args.push_back("mycmd");
    args.push_back("mycmd2");
    args.push_back("mycmd3");

    args.push_back("-tt");
    args.push_back("10");

    args.push_back("-n");
    args.push_back("9");
    args.push_back("pp");

    args.push_back("-sa");
    args.push_back("pp");

    args.push_back("-hc");
    args.push_back("01");
    args.push_back("00");
    args.push_back("00");
    args.push_back("00");

    liblz::CArgOpt opt(args);
    opt.parse_options(args);
    opt.show_options();

    if (opt.has_option("-sa"))
    {
        cout << "has sa" << endl;
    }
    else
    {
        cout << "no sa" << endl;
    }

    string s = opt.get_string("-sa", "sss");
    std::cout << "s=" << s << std::endl;

    float a = opt.get_float("-sa", 6.6);
    std::cout << "a=" << a << std::endl;

    int b = opt.get_int("-sa", 6.6);
    std::cout << "b=" << b << std::endl;

    unsigned long c = opt.get_ulong("-sa", 6.6);
    std::cout << "c=" << c << std::endl;
}
__TEST_FUNC_END__

__TEST_FUNC_BEGIN__(test)
{
    liblz::CArgOpt opt;

    std::vector<std::string> args;
    args.push_back("mycmd");
    args.push_back("mycmd2");
    args.push_back("mycmd3");

    args.push_back("-tt");
    args.push_back("10");

    args.push_back("-n");
    args.push_back("9");
    args.push_back("pp");

    args.push_back("-sa");

    args.push_back("-hc");
    args.push_back("01");
    args.push_back("00");
    args.push_back("00");
    args.push_back("00");

    opt.parse_options(args);
    opt.show_options();
}
__TEST_FUNC_END__

int main(int argc, char *argv[])
{
    test();
    test_1();
    test2();
    return 0;
}
