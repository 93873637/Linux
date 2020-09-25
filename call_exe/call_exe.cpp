#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int run_app_by_system(const char *app_name)
{
    pid_t status = system(app_name);
    if (-1 == status)
    {
        //printf("system error!");
        return -1;
    }
    
    //printf("exit status value = [0x%x]\n", status);
    if (WIFEXITED(status))
    {
        if (0 == WEXITSTATUS(status))
        {
            //printf("run shell script successfully.\n");
            return 0;
        }
        else
        {
            //printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
            return WEXITSTATUS(status);
        }
    }
    else
    {
        //printf("exit status = [%d]\n", WEXITSTATUS(status));
        return -2;
    }
}

int main()
{
    const char *app_name = "lpp";
    printf("run app exit with %d\n", run_app_by_system(app_name));
    
//    pid_t status;
// 
// 
//    status = system("ls");
// 
//    if (-1 == status)
//    {
//        printf("system error!");
//    }
//    else
//    {
//        printf("exit status value = [0x%x]\n", status);
// 
//        if (WIFEXITED(status))
//        {
//            if (0 == WEXITSTATUS(status))
//            {
//                printf("run shell script successfully.\n");
//            }
//            else
//            {
//                printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
//            }
//        }
//        else
//        {
//            printf("exit status = [%d]\n", WEXITSTATUS(status));
//        }
//    }
 
    return 0;
}
