/**
 * TimeUtil.h: header functions to time utils
 * 
 * tom.li@cloudminds.com
 * 20/07/20/
 */

#ifndef __TIME_UTILS_H__
#define __TIME_UTILS_H__

#include <errno.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <typeinfo>
#ifndef _WINDOWS
#include <unistd.h>
#endif

using namespace std;

#define SLEEP_S(_s) usleep(_s * 1e6)
#define SLEEP_MS(_ms) usleep(_ms * 1e3)
#define SLEEP_US(_us) usleep(_us)

static __inline int get_time_ms()
{
#ifdef _WINDOWS
    return 0;
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (int64_t)now.tv_sec * 1000 + now.tv_nsec / 1000000;
#endif
}

static __inline int64_t get_time_us()
{
#ifdef _WINDOWS
    return 0;
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (int64_t)now.tv_sec * 1000000 + now.tv_nsec / 1000;
#endif
}

static __inline void get_times(int& dd, int &hh, int &mm, int &ss, int &ms, int &us)
{
#ifdef _WINDOWS
    return 0;
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    dd = now.tv_sec / 3600 / 24;
    hh = now.tv_sec / 3600 % 24;
    mm = now.tv_sec / 60 % 60;
    ss = now.tv_sec - now.tv_sec / 60 * 60;
    ms = now.tv_nsec / 1000000;
    us = (now.tv_nsec % 1000000) / 1000;
#endif
}

#define BUILD_TIMES             \
    int dd, hh, mm, ss, ms, us; \
    get_times(dd, hh, mm, ss, ms, us)

#define PRINT_TIME_FORMAT  "%d.%02d:%02d:%02d.%03d.%03d"
#define PRINT_TIME_VARS  dd, hh, mm, ss, ms, us

long current_time_ms();
std::string &get_format_time(std::string &time_str);
std::string &get_format_time_ms(std::string &time_str);

#endif // __TIME_UTILS_H__
