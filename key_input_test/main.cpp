// #include <stdio.h>
// #include "CmdInput.h"

// int main(int argc, char** argv)
// {
//     printf("abc\b");
//     printf("\n");
//     CmdInput.enter_cmd_loop();
//     return(0);
// }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 线控
// 控制与终端的连接

// tcsendbreak 如果终端工作在异步串行数据发送模式，在指定时长内发送0比特流。如果时长为0，发送0比特流至少0.25s，但不多于0.5s。如果终端未工作在异步串行数据发送模式，tcsendbreak立刻返回不作任何操作。
// tcdrain() 等待直到所有输出至fd的数据均被发送
// tcflush() 丢弃已写入fd但还未发送的数据，或丢弃已接收但还未读取的数据。丢弃对象取决于queue_selector：
// TCIFLUSH 丢弃已接收但还未读取的数据
// TCOFLUSH 丢弃已写入但还未发送的数据
// TCIOFLUSH 丢弃以上两种
// tcflow() 挂起fd的发送或接收操作。挂起对象取决于action:
// TCOOFF 挂起输出
// TCOON 重启挂起的输出
// TCIOFF 发送STOP字符，停止终端设备向系统发送数据
// TCION 发送START字符，启动设备向系统发送数据
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// 下面的例子程序获取标准输入终端(STDIN)的属性并更改标准输入终端为raw模式(也可以使用更简便的调用－cfmakeraw()完成)，接收键盘输入、显示键值，直到接收到Ctrl-b输入。
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

#define BUF_SIZE 255

#define INVISIBLE_CHAR    0x80

int main(void)
{
    int ret = 0;
    char buf[BUF_SIZE] = {0};
    int i = 0;

    struct termios newtmios = {0};
    struct termios oldtmios = {0};

    ret = tcgetattr(STDIN_FILENO, &oldtmios);
    if (ret)
    {
        printf("tcgetattr() error, errno = 0x%X\n", errno);
        return -1;
    }

    memcpy(&newtmios, &oldtmios, sizeof(struct termios));
    newtmios.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    newtmios.c_oflag &= ~OPOST;
    newtmios.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    newtmios.c_cflag &= ~(CSIZE | PARENB);
    newtmios.c_cflag |= CS8;

    ret = tcsetattr(STDIN_FILENO, TCSANOW, &newtmios);
    if (ret)
    {
        printf("tcsetattr() error, errno = 0x%X\n", errno);
        return -2;
    }

    printf("Press any key to test(ctrl-b/ctrl-c to quit): \r\n");
    fflush(stdout);
    //tcdrain(STDIN_FILENO);
    //tcflush(STDIN_FILENO, TCIOFLUSH);

    while (1)
    {
        ret = read(STDIN_FILENO, buf, sizeof(buf));
        if (ret)
        {
            //printf("rread %d chars, you pressed ", ret);
            for (i = 0; i < ret; i++)
            {
                // printf(" 0x%02X ", buf[i]);
                printf(" (0x%02X %3d '%c') ", buf[i], buf[i], (isprint(buf[i])!=0)?buf[i]:INVISIBLE_CHAR);
            }
            printf("\r\n");
            fflush(stdout);
            //tcflush(STDOUT_FILENO, TCIOFLUSH);
            //tcflush(STDIN_FILENO, TCIOFLUSH);

            if ((1 == ret) &&
                (buf[0] == 0x02 /*ctrl+b*/ || buf[0] == 0x03 /*ctrl+c*/))
            {
                break;
            }
        }
    }

    ret = tcsetattr(STDIN_FILENO, TCSANOW, &oldtmios);
    return ret;
}
