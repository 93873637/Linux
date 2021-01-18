///////////////////////////////////////////////////////////////////////////////////////////////////
// TimeUtils.cpp:

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include "TimeUtils.h"

using namespace std;

/**
 * current time from system power up, unit by milli-seconds
 */
long current_time_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long)ts.tv_sec * 1000 + (long)ts.tv_nsec / 1000000;
}

/**
 * format time string as yy.mmdd.HHMMSS
 */
std::string &get_format_time(std::string &time_str)
{
    char time_buf[16] = {0};
    const time_t &t = time(NULL);
    strftime(time_buf, sizeof(time_buf), "%y.%m%d.%H%M%S", localtime(&t));
    time_str = time_buf;
    return time_str;
}

/**
 * format time string as yy.mmdd.HHMMSS
 */
std::string &get_format_time_ms(std::string &time_str)
{
    struct timeb tp;
    char ti[32];
    ftime(&tp);
    strftime(ti, sizeof(ti), "%y.%m%d.%H%M%S", localtime(&tp.time));
    char ms[8];
    sprintf(ms, "%03u", tp.millitm);
    time_str = ti;
    time_str += ".";
    time_str += ms;
    return time_str;
}
