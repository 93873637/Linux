#include "log.h"

using namespace utils::log;
int
main(int argc, char* argv[])
{
    SetupLogging(argc, argv, true);

    LOG(INFO) << APP_NAME;
    LOG(INFO) << "HELLO WORLD1111";
    return 0;
}
