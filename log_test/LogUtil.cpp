#include <algorithm>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "FileUtil.h"
#include "LogUtil.h"
#include "ProcUtil.h"
#include "StrUtil.h"
#include "TimeUtil.h"

using namespace std;

#define CHECK_LOG_LEVEL     \
    if (level < log_level_) \
    {                       \
        return;             \
    }

#define OPEN_LOG_FILE                                                \
    CLOSE_LOG_FILE;                                                  \
    log_file_ = NEW_OBJ File;                                        \
    const char *new_logfile_name = generate_log_file_name();         \
    if (!log_file_->open(new_logfile_name, IO_MODE_REWR_ORNEW))      \
    {                                                                \
        PRINT_E("open new log file %s failed.\n", new_logfile_name); \
        DEL_OBJ(log_file_);                                          \
        return false;                                                \
    }                                                                \
    PRINT_I("open new log file %s\n", new_logfile_name);

#define CLOSE_LOG_FILE                                                \
    if (log_file_ != NULL)                                            \
    {                                                                 \
        PRINT_I("close log file %s\n", log_file_->getName().c_str()); \
        log_file_->close();                                           \
        DEL_OBJ(log_file_);                                           \
    }

#define IS_LOG_ON (log_file_ != NULL)

namespace libemb
{

Logger::Logger()
    : log_level_(LOG_LEVEL_DEF), remote_level_(LOG_LEVEL_I), log_file_(NULL), can_hex_(false)
{
    print_buf_ = new char[PRINT_BUF_SIZE];
}

Logger::~Logger()
{
    delete[] print_buf_;
}

void Logger::print(int level, const char *format, ...)
{
    if (level < log_level_)
    {
        return;
    }

    char *buf = print_buf_;

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

        if (log_file_ != NULL && log_level_ >= LOG_LEVEL_FILE)
        {
            if (log_file_check())
            {
                log_file_->writeData(buf, size);
            }
        }
    }
}

void Logger::print_hex(int level, const char *tag, const char *buf, int len)
{
    print_hex(level, tag, (const unsigned char *)buf, len);
}

void Logger::print_hex(int level, const char *tag, const unsigned char *buf, int len)
{
    CHECK_LOG_LEVEL;
    if (buf == NULL || len <= 0)
    {
        return;
    }
    printf("%012lld %s[%d] = {", TIME_US(), tag, len);
    for (int i = 0; i < len - 1; i++)
    {
        printf("%02X ", *(buf + i));
    }
    printf("%02X}\n", *(buf + len - 1));
}

const char *Logger::to_hex_str(char *buf, const int max_buf_len, const unsigned char *data, const int data_len)
{
    if (max_buf_len < data_len * 3) //02x+1space for one char, last one end with '\0'
    {
        return "";
    }
    char *pb = buf;
    int i, n;
    for (i = 0; i < data_len - 1; i++)
    {
        n = sprintf(pb, "%02X ", data[i]);
        pb += n;
    }
    n = sprintf(pb, "%02X", data[i]);
    pb += n;
    *(++pb) = '\0';
    return buf;
}

void Logger::log_status()
{
    printf("log status: %s\n", IS_LOG_ON ? "ON" : "OFF");
    printf("log level: %s(%d)\n", LOG_LEVEL_NAME(log_level_), log_level_);
    printf("log file: %s\n", log_file_ == NULL ? "NULL" : log_file_->getName().c_str());
}

void Logger::can_hex_status()
{
    printf("can hex status: %s\n", can_hex_ ? "ON" : "OFF");
}

bool Logger::open_log()
{
    //##@: 
    // getenv leads to exception in am.service, using static dir instead
    //std::string log_dir = getenv("HOME");
    //log_dir += "/log";
    //return open_log(log_dir.c_str(), "sca");
    return open_log(DEFAULT_LOG_DIR, DEFAULT_LOG_PREFIX);
}

bool Logger::open_log(const char *dir_name, const char *prefix)
{
    assert(dir_name != NULL);
    assert(prefix != NULL);
    log_dir_ = dir_name;
    log_file_prefix_ = prefix;
    Directory dir;
    if (!dir.exists(log_dir_.c_str()))
    {
        PRINT_I("log dir \"%s\" not exist, create...\n", log_dir_.c_str());
        if (!dir.createDir(log_dir_.c_str()))
        {
            PRINT_E("create log dir \"%s\" failed.\n", log_dir_.c_str());
            return false;
        }
    }
    OPEN_LOG_FILE;
    return true;
}

void Logger::close_log()
{
    CLOSE_LOG_FILE;
}

const char *Logger::generate_log_file_name()
{
    std::string time_str;
    log_file_name_ = log_dir_ + "/" + log_file_prefix_ + "_" + get_format_time_ms(time_str) + ".log";
    return log_file_name_.c_str();
}

bool Logger::log_file_check()
{
    if (log_file_ == NULL)
    {
        PRINT_E("log file null\n");
        return false;
    }

    // check if current file exceed max size
    int log_file_size = log_file_->getSize();
    if (log_file_size == STATUS_ERROR)
    {
        PRINT_E("get log file size failed\n");
        return false;
    }
    if (log_file_size < MAX_LOG_FILE_SIZE)
    {
        // still use current file
        return true;
    }
    PRINT_I("log file size %d exceed max %d\n", log_file_->getSize(), MAX_LOG_FILE_SIZE);

    // up to max log file size, we need create a new log
    // first delete the oldest log file if log files number up to max
    Directory dir;
    dir.enter(log_dir_);
    std::vector<std::string> dir_files = dir.listAll();
    std::vector<std::string> log_files;
    for (size_t i = 0; i < dir_files.size(); i++)
    {
        if (dir_files[i].find(log_file_prefix_) == 0)
        {
            log_files.push_back(dir_files[i]);
        }
    }

    int num = log_files.size();
    PRINT_I("current log file number: %d\n", log_files.size());
    if (num < MAX_LOG_FILE_NUM)
    {
        // not exceed max log file number, create a new log directly
        OPEN_LOG_FILE;
        return true;
    }

    // delete old files until file num less than max
    std::sort(log_files.begin(), log_files.end());
    while (log_files.size() >= MAX_LOG_FILE_NUM)
    {
        std::string old_file = log_dir_ + "/" + log_files.front();
        PRINT_I("current log file number %d exceed max %d, delete file: %s\n", log_files.size(), MAX_LOG_FILE_NUM, old_file.c_str());
        if (!libemb::File::deleteFile(old_file.c_str()))
        {
            PRINT_E("delete old file %s failed\n", old_file.c_str());
            return false;
        }
        log_files.erase(log_files.begin());
    }

    OPEN_LOG_FILE;
    return true;
}

bool Logger::remoteControl(const char *ctrlMsg, int len, Logger_CallCmd callcmd /*=NULL*/)
{
    /* 协议文本“1:4:/dev/pts/x” */
    std::string msg(ctrlMsg, len);
    if (msg.size() < 14)
    {
        return false;
    }
    if (msg.substr(3, 10) != ":/dev/pts/")
    {
        if (msg.substr(3, 10) == ":>>>cmd<<<" && callcmd != NULL)
        {
            std::string cmd = msg.substr(13);
            callcmd(CSTR(cmd));
            return true;
        }
        return false;
    }
    bool enable = !!StrUtil::stringToInt(msg.substr(0, 1));
    int level = StrUtil::stringToInt(msg.substr(2, 1));
    std::string ptsDev = msg.substr(4);
    if (enable)
    {
        remote_level_ = log_level_;
        set_level(level);
#if 0
        close(1);
	    close(2);
	    /* 在新终端打开标准输出 */
	    int ret = (int)open(CSTR(ptsDev), 1);
	    if(ret<0)
	    {
	         LOG_E("redirect stdout to %s error:%s.\n",CSTR(ptsDev), ERR_STR);
	         return false;    
	    }
	    /* 在新终端打开标准错误输出 */
	    ret = (int)open(CSTR(ptsDev), 2);
	    if(ret<0)
	    {
	        LOG_E("redirect stderr to %s error:%s.\n", CSTR(ptsDev), ERR_STR); 
	        return false;
	    }
	    fflush(NULL);
		LOG_E("ctrace setup, level:%d\n",level);
#endif
        return true;
    }
    else
    {
        set_level(remote_level_);
#if 0 /* 在ECU项目中,ctrace退出时会导致SCA亮蓝灯,暂未查找到原因,因此屏蔽此段代码 */
        std::string ttyName = ProcUtil::execute("tty");
	    if (ttyName.empty()) {
	        return false;
	    }
	    ttyName = StrUtil::trimTailBlank(ttyName);
	    /* 在当前终端重新打开标准输出和标准错误输出 */
	    freopen(CSTR(ttyName),"w",stdout);
	    freopen(CSTR(ttyName),"w",stderr);
	    /* 不知道为什么会产生这样一个文件!只能强制把它删除! */
	    ProcUtil::execute("rm -rf not\\ a\\ tty ");
		LOG_E("ctrace quit, level:%d\n",remote_level_);
#endif
        return true;
    }
}

} // namespace libemb
