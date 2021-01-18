#include "TimeUtils.h"
#include <stdio.h>

/*
 * 显示：0(默认)、1(粗体/高亮)、22(非粗体)、4(单条下划线)、24(无下划线)、5(闪烁)、25(无闪烁)、7(反显、翻转前景色和背景色)、27(无反显)
 * 颜色值：0(黑)、1(红)、2(绿)、 3(黄)、4(蓝)、5(紫)、6(深绿)、7(白)
 * 颜色分为背景色和字体色，30~39（30+颜色值）用来设置字体色（前景色），40~49（40+颜色值）设置背景（背景色）：如31表示前景色为红色，41表示背景色为红色。
 */
#define COLOR_NONE         			"\033[0;m"
#define RED          				"\033[0;31m"
#define LIGHT_RED    				"\033[1;31m"  //红色高亮
#define LIGHT_RED_INV    			"\033[5;7;31m"  //红色高亮，并反白显示，字体闪烁
#define GREEN        				"\033[0;32m"
#define LIGHT_GREEN  				"\033[1;32m"
#define BLUE         				"\033[0;34m"
#define LIGHT_BLUE   				"\033[1;34m" //蓝色高亮
#define DARY_GRAY    				"\033[1;30m"
#define CYAN         				"\033[0;36m"
#define LIGHT_CYAN   				"\033[1;36m"
#define PURPLE       				"\033[0;35m"
#define LIGHT_PURPLE 				"\033[1;35m"
#define YELLOW        				"\033[0;33m"
#define LIGHT_YELLOW       			"\033[1;33m" //黄色高亮
#define WHITE                       "\033[0;37m"
#define LIGHT_WHITE        			"\033[1;37m" //白色高亮

int main()
{
    printf("main: E...\n");

    // for (int i = 0; i < 100; i++)
    // {
    //     printf("%09d: this is time_ms\n", get_time_ms());
    // }

    // for (int i = 0; i < 100; i++)
    // {
    //     printf("%012ld: this is time_us\n", get_time_us());
    // }

    int count = 0;
    for (int i = 0; i < 1000; i++)
    {
        BUILD_TIMES;
        //printf("%06d: %d.%02d:%02d:%02d.%03d.%03d: this is dd-hh:mm:ss.ms.us\n", ++count, dd, hh, mm, ss, ms, us);
        printf("%06d: " PRINT_TIME_FORMAT ": this is dd-hh:mm:ss.ms.us\n", ++count, PRINT_TIME_VARS);
        //SLEEP_US(1000);
    }

    printf("main: X.\n");
}
