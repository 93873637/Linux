#ifndef __LOG_UTILS_H__
#define __LOG_UTILS_H__

#include "BaseType.h"
#include "FileUtil.h"
#include "ProcUtil.h"
#include "Thread.h"
#include "ThreadUtil.h"
#include <errno.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <typeinfo>

#ifdef WIN32
#define FILENAME(x) strrchr(x, '\\') ? strrchr(x, '\\') + 1 : x
#else
#define FILENAME(x) strrchr(x, '/') ? strrchr(x, '/') + 1 : x
#endif

#define PRINT_BUF_SIZE 4096 /* max length for print once, use "..." on exceed */
#define MAX_LOG_FILE_NUM 5
#define MAX_LOG_FILE_SIZE 2000000 // unit by bytes

static __inline int TIME_MS()
{
#ifdef _WINDOWS
    return 0;
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (int64_t)now.tv_sec * 1000 + now.tv_nsec / 1000000;
#endif
}

static __inline int64_t TIME_US()
{
#ifdef _WINDOWS
    return 0;
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (int64_t)now.tv_sec * 1000000 + now.tv_nsec / 1000;
#endif
}

__inline static void print_color(const char *format, ...)
{
    char buf[PRINT_BUF_SIZE];
    va_list argp;
    va_start(argp, format);
    int size = vsnprintf(buf, PRINT_BUF_SIZE, format, argp);
    va_end(argp);
    if (size > 0)
    {
        if (size >= PRINT_BUF_SIZE)
        {
            buf[PRINT_BUF_SIZE - 5] = '.';
            buf[PRINT_BUF_SIZE - 4] = '.';
            buf[PRINT_BUF_SIZE - 3] = '.';
            buf[PRINT_BUF_SIZE - 2] = '\n';
            buf[PRINT_BUF_SIZE - 1] = '\0';
            size = PRINT_BUF_SIZE;
        }
        write(fileno(stdout), buf, size);
        fflush(stdout);
    }
}

#define PRINT_BLACK(_fmt, _arg...) print_color("\033[30m\033[1m" _fmt "\033[0m", ##_arg)
#define PRINT_RED(_fmt, _arg...) print_color("\033[31m\033[1m" _fmt "\033[0m", ##_arg)
#define PRINT_GREEN(_fmt, _arg...) print_color("\033[32m\033[1m" _fmt "\033[0m", ##_arg)
#define PRINT_YELLOW(_fmt, _arg...) print_color("\033[33m\033[1m" _fmt "\033[0m", ##_arg)
#define PRINT_BLUE(_fmt, _arg...) print_color("\033[34m\033[1m" _fmt "\033[0m", ##_arg)
#define PRINT_PURPLE(_fmt, _arg...) print_color("\033[35m\033[1m" _fmt "\033[0m", ##_arg)
#define PRINT_CYAN(_fmt, _arg...) print_color("\033[36m\033[1m" _fmt "\033[0m", ##_arg)
#define PRINT_WHITE(_fmt, _arg...) print_color("\033[37m\033[1m" _fmt "\033[0m", ##_arg)

#define PRINT_V PRINT_WHITE
#define PRINT_D PRINT_CYAN
#define PRINT_I PRINT_GREEN
#define PRINT_W PRINT_YELLOW
#define PRINT_E PRINT_RED

#define LOG_LEVEL_V 0 /* verbose */
#define LOG_LEVEL_T 1 /* trace */
#define LOG_LEVEL_D 2 /* debug */
#define LOG_LEVEL_I 3 /* info */
#define LOG_LEVEL_W 4 /* warning */
#define LOG_LEVEL_E 5 /* error */
#define LOG_LEVEL_P 6 /* print out */

#define LOG_LEVEL_MIN LOG_LEVEL_V
#define LOG_LEVEL_MAX LOG_LEVEL_P
#define LOG_LEVEL_DEF LOG_LEVEL_D /* default level */

#define LOG_LEVEL_FILE LOG_LEVEL_D /* min level for write log to file */

#define LOG_BLACK(_level, _fmt, _arg...)                                                 \
    do                                                                                   \
    {                                                                                    \
        libemb::Logger::inst()->print(_level, "\033[30m\033[1m" _fmt "\033[0m", ##_arg); \
    } while (0)
#define LOG_RED(_level, _fmt, _arg...)                                                   \
    do                                                                                   \
    {                                                                                    \
        libemb::Logger::inst()->print(_level, "\033[31m\033[1m" _fmt "\033[0m", ##_arg); \
    } while (0)
#define LOG_GREEN(_level, _fmt, _arg...)                                                 \
    do                                                                                   \
    {                                                                                    \
        libemb::Logger::inst()->print(_level, "\033[32m\033[1m" _fmt "\033[0m", ##_arg); \
    } while (0)
#define LOG_YELLOW(_level, _fmt, _arg...)                                                \
    do                                                                                   \
    {                                                                                    \
        libemb::Logger::inst()->print(_level, "\033[33m\033[1m" _fmt "\033[0m", ##_arg); \
    } while (0)
#define LOG_BLUE(_level, _fmt, _arg...)                                                  \
    do                                                                                   \
    {                                                                                    \
        libemb::Logger::inst()->print(_level, "\033[34m\033[1m" _fmt "\033[0m", ##_arg); \
    } while (0)
#define LOG_PURPLE(_level, _fmt, _arg...)                                                \
    do                                                                                   \
    {                                                                                    \
        libemb::Logger::inst()->print(_level, "\033[35m\033[1m" _fmt "\033[0m", ##_arg); \
    } while (0)
#define LOG_CYAN(_level, _fmt, _arg...)                                                  \
    do                                                                                   \
    {                                                                                    \
        libemb::Logger::inst()->print(_level, "\033[36m\033[1m" _fmt "\033[0m", ##_arg); \
    } while (0)
#define LOG_WHITE(_level, _fmt, _arg...)                                                 \
    do                                                                                   \
    {                                                                                    \
        libemb::Logger::inst()->print(_level, "\033[37m\033[1m" _fmt "\033[0m", ##_arg); \
    } while (0)

#define LOG_COLOR_V LOG_WHITE
#define LOG_COLOR_D LOG_CYAN
#define LOG_COLOR_I LOG_GREEN
#define LOG_COLOR_W LOG_YELLOW
#define LOG_COLOR_E LOG_RED

#define LOG_V(_fmt, _arg...)                                                                                              \
    {                                                                                                                     \
        LOG_WHITE(LOG_LEVEL_V, "%012lld V %s:%d/%s - " _fmt, TIME_US(), FILENAME(__FILE__), __LINE__, __FUNCTION__, ##_arg); \
    }
#define LOG_D(_fmt, _arg...)                                                                                             \
    {                                                                                                                    \
        LOG_CYAN(LOG_LEVEL_D, "%012lld D %s:%d/%s - " _fmt, TIME_US(), FILENAME(__FILE__), __LINE__, __FUNCTION__, ##_arg); \
    }
#define LOG_I(_fmt, _arg...)                                                                                              \
    {                                                                                                                     \
        LOG_GREEN(LOG_LEVEL_I, "%012lld I %s:%d/%s - " _fmt, TIME_US(), FILENAME(__FILE__), __LINE__, __FUNCTION__, ##_arg); \
    }
#define LOG_W(_fmt, _arg...)                                                                                               \
    {                                                                                                                      \
        LOG_YELLOW(LOG_LEVEL_W, "%012lld W %s:%d/%s - " _fmt, TIME_US(), FILENAME(__FILE__), __LINE__, __FUNCTION__, ##_arg); \
    }
#define LOG_E(_fmt, _arg...)                                                                                            \
    {                                                                                                                   \
        LOG_RED(LOG_LEVEL_E, "%012lld E %s:%d/%s - " _fmt, TIME_US(), FILENAME(__FILE__), __LINE__, __FUNCTION__, ##_arg); \
    }

#define LOG_T(_level)                                                                                                                 \
    {                                                                                                                                 \
        LOG_COLOR_##_level(LOG_LEVEL_##_level, "%012lld " #_level " %s:%d/%s\n", TIME_US(), FILENAME(__FILE__), __LINE__, __FUNCTION__); \
    }
#define LOG_TV LOG_T(V)
#define LOG_TD LOG_T(D)
#define LOG_TI LOG_T(I)
#define LOG_TW LOG_T(W)
#define LOG_TE LOG_T(E)

#define LOG_IE LOG_I("E...\n")
#define LOG_IX LOG_I("X.\n")

#define LOG_DE LOG_D("E...\n")
#define LOG_DX LOG_D("X.\n")

#define LOG_P(_fmt, _arg...)                 \
    {                                        \
        LOG_BLUE(LOG_LEVEL_P, _fmt, ##_arg); \
    }
#define LOG_PV(_fmt, _arg...)                 \
    {                                         \
        LOG_WHITE(LOG_LEVEL_V, _fmt, ##_arg); \
    }
#define LOG_PD(_fmt, _arg...)                \
    {                                        \
        LOG_CYAN(LOG_LEVEL_D, _fmt, ##_arg); \
    }
#define LOG_PI(_fmt, _arg...)                 \
    {                                         \
        LOG_GREEN(LOG_LEVEL_I, _fmt, ##_arg); \
    }
#define LOG_PW(_fmt, _arg...)                  \
    {                                          \
        LOG_YELLOW(LOG_LEVEL_W, _fmt, ##_arg); \
    }
#define LOG_PE(_fmt, _arg...)               \
    {                                       \
        LOG_RED(LOG_LEVEL_E, _fmt, ##_arg); \
    }

#define LOG_HEX(tag, buf, len)                                         \
    do                                                                 \
    {                                                                  \
        libemb::Logger::inst()->print_hex(LOG_LEVEL_D, tag, buf, len); \
    } while (0)

#define LOG_TEXT(_fmt, _arg...)                                     \
    do                                                              \
    {                                                               \
        if (libemb::Logger::inst()->getLevel() == LOG_LEVEL_D)      \
        {                                                           \
            printf("----------------------------------------\n");   \
            printf(_fmt, _arg);                                     \
            printf("\n----------------------------------------\n"); \
        }                                                           \
    } while (0)

#define LOG_TRACE                                                                                            \
    {                                                                                                        \
        LOG_PURPLE(LOG_LEVEL_T, "%012lld T %s:%d/%s\n", TIME_US(), FILENAME(__FILE__), __LINE__, __FUNCTION__); \
    }
#define LOG_TRACE_E                                                                                                 \
    {                                                                                                               \
        LOG_PURPLE(LOG_LEVEL_T, "%012lld T %s:%d/%s - E...\n", TIME_US(), FILENAME(__FILE__), __LINE__, __FUNCTION__); \
    }
#define LOG_TRACE_X                                                                                               \
    {                                                                                                             \
        LOG_PURPLE(LOG_LEVEL_T, "%012lld T %s:%d/%s - X.\n", TIME_US(), FILENAME(__FILE__), __LINE__, __FUNCTION__); \
    }
#define LOG_TRACE_MSG(_fmt, _arg...)                                                                                    \
    {                                                                                                                   \
        LOG_PURPLE(LOG_LEVEL_T, "%012lld T %s:%d/%s" _fmt, TIME_US(), FILENAME(__FILE__), __LINE__, __FUNCTION__, ##_arg); \
    }

#define LOG_TODO LOG_W("TODO: UNIMPLEMENTED\n")

#define SET_LOG_LEVEL(_level) Logger::inst()->set_level(_level)
#define SET_LOG_LEVEL_MAX SET_LOG_LEVEL(LOG_LEVEL_E)
#define SET_LOG_LEVEL_MIN SET_LOG_LEVEL(LOG_LEVEL_V)
#define SET_LOG_LEVEL_D SET_LOG_LEVEL(LOG_LEVEL_D)
#define SET_LOG_LEVEL_E SET_LOG_LEVEL(LOG_LEVEL_E)

#define CLASS_NAME(classPtr) (&(typeid(*classPtr).name()[1]))

#define LOG_STATUS Logger::inst()->log_status()
#define LOG_ON                        \
    {                                 \
        Logger::inst()->open_log();   \
        Logger::inst()->log_status(); \
    }
#define LOG_OFF                       \
    {                                 \
        Logger::inst()->close_log();  \
        Logger::inst()->log_status(); \
    }

#define CAN_HEX_STATUS Logger::inst()->can_hex_status()
#define CAN_HEX_ON                         \
    {                                      \
        Logger::inst()->set_can_hex(true); \
        Logger::inst()->can_hex_status();  \
    }
#define CAN_HEX_OFF                         \
    {                                       \
        Logger::inst()->set_can_hex(false); \
        Logger::inst()->can_hex_status();   \
    }

#define DEFAULT_LOG_DIR "/home/root/log"
#define DEFAULT_LOG_PREFIX "sca"

__inline static const char *LOG_LEVEL_NAME(int level)
{
    switch (level)
    {
    case LOG_LEVEL_V:
        return "LOG_LEVEL_V";
    case LOG_LEVEL_T:
        return "LOG_LEVEL_T";
    case LOG_LEVEL_D:
        return "LOG_LEVEL_D";
    case LOG_LEVEL_I:
        return "LOG_LEVEL_I";
    case LOG_LEVEL_W:
        return "LOG_LEVEL_W";
    case LOG_LEVEL_E:
        return "LOG_LEVEL_E";
    case LOG_LEVEL_P:
        return "LOG_LEVEL_P";
    default:
        return "UNKNOWN";
    }
}

typedef void (*Logger_CallCmd)(const char *cmd);

namespace libemb
{

class Logger
{
public:
    static Logger *inst()
    {
        static Logger inst_;
        return &inst_;
    }

protected:
    Logger();
    ~Logger();

public:
    void print(int level, const char *format, ...);
    void print_hex(int level, const char *tag, const unsigned char *buf, int len);
    void print_hex(int level, const char *tag, const char *buf, int len);
    static const char *to_hex_str(char *buf, const int max_buf_len, const unsigned char *data, const int data_len);

public:
    int log_level() { return log_level_; }
    void set_level(int level) { log_level_ = level; }
    bool is_debugging() { return (log_level_ <= LOG_LEVEL_D); }
    bool can_hex() { return can_hex_; }
    void set_can_hex(bool on) { can_hex_ = on; }
    void can_hex_status();

    // log file options
public:
    void log_status();
    bool open_log();
    bool open_log(const char *dir_name, const char *prefix);
    void close_log();
    const char *generate_log_file_name();
    bool log_file_check();
    bool remoteControl(const char *ctrlMsg, int len, Logger_CallCmd callcmd = NULL);

private:
    int log_level_;
    int remote_level_;
    char *print_buf_;
    std::string log_file_name_;
    File *log_file_;
    std::string log_dir_;
    std::string log_file_prefix_;
    bool can_hex_;
};

} //namespace libemb

#endif // __LOG_UTILS_H__
