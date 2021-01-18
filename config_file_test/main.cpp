
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "SCAConfig.h"

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

__TEST_FUNC_BEGIN__(test)
{
    SCAConfig.init();
}
__TEST_FUNC_END__

int main(int argc, char *argv[])
{
    test();
    return 0;
}
