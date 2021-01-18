#ifndef UTILS_LOG_H_
#define UTILS_LOG_H_
#include <glog/logging.h>

namespace utils
{
namespace log
{
void SetupLogging(int argc, char** argv, bool save);
} // namespace log
} // namespace utils
#endif // UTILS_LOG_H_