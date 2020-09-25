#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "KeyInput.h"

static void on_sigint(int sig)
{
    printf("on_sigint\n");
    KeyInput.exit_key_loop();
}

//////////////////////////////////////////////////////////////////////////
// Singleton Decl.

CKeyInput *CKeyInput::inst_ = NULL;
CKeyInput *CKeyInput::inst()
{
    if (CKeyInput::inst_ == NULL)
    {
        CKeyInput::inst_ = new CKeyInput();
    }
    return CKeyInput::inst_;
}

CKeyInput::CKeyInput()
    : cont_loop_(false)
{
}

CKeyInput::~CKeyInput()
{
}

// Singleton Decl.
//////////////////////////////////////////////////////////////////////////

/*
 * "Timed" read - timout specifies the # of seconds to wait before
 * giving up (5th argument to select controls how long to wait for
 * data to be readable).  Returns # of bytes read or -1 on error.
 */
ssize_t
tread(int fd, void *buf, size_t nbytes, unsigned int timout)
{
    int nfds;
    fd_set readfds;
    struct timeval tv;
    tv.tv_sec = timout;
    tv.tv_usec = 0;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    nfds = select(fd+1, &readfds, NULL, NULL, &tv);
    if (nfds < 0)
    {
        printf("select error %d\n", nfds);
        return(-1);
    }
    else if (nfds == 0)
    {
        //printf("select timeout\n");
        return 0;
    }
    else
    {
        return(read(fd, buf, nbytes));
    }
}

// /*
//  * "Timed" read - timout specifies the number of seconds to wait
//  * per read call before giving up, but read exactly nbytes bytes.
//  * Returns number of bytes read or -1 on error.
//  *
//  * LOCKING: none.
//  */
// ssize_t
// treadn(int fd, void *buf, size_t nbytes, unsigned int timout)
// {
//     size_t    nleft;
//     ssize_t    nread;
//     nleft = nbytes;
//     while (nleft > 0) {
//         if ((nread = tread(fd, buf, nleft, timout)) < 0) {
//             if (nleft == nbytes)
//                 return(-1); /* error, return -1 */
//             else
//                 break;      /* error, return amount read so far */
//         } else if (nread == 0) {
//             break;          /* EOF */
//         }
//         nleft -= nread;
//         buf += nread;
//     }
//     return(nbytes - nleft);      /* return >= 0 */
// }

int CKeyInput::key_loop()
{
    printf("CKeyInput::key_Loop: E...\n");

    signal(SIGINT, on_sigint);  // Ctrl+c: force break
    signal(SIGQUIT, SIG_IGN);  // Ctrl+4: quit
    signal(SIGHUP, SIG_IGN);  // Ctrl+z
  
    tcgetattr(KEYBOARD_FD, &org_settings_);

    struct termios new_settings;
    memcpy(&new_settings, &org_settings_, sizeof(struct termios));
    new_settings.c_lflag &=~(ICANON | ECHO);
    new_settings.c_cc[VEOL] = 1;
    new_settings.c_cc[VEOF] = 2;
    tcsetattr(KEYBOARD_FD, TCSANOW, &new_settings);

    int ret;
    char c;
    cont_loop_ = true;
    while (cont_loop_)
    {
        ret = tread(KEYBOARD_FD, &c, 1, 1);
        if (ret < 0)
        {
            printf("read error %d\n", ret);
            break;
        }
        else if (ret == 0)
        {
            //printf("read timeout\n");
            continue;
        }
        else
        {
            handle_key_event(c);
        }
    }

    printf("restore original settings...\n");
    tcsetattr(KEYBOARD_FD, TCSANOW, &org_settings_);
    
    printf("cont_loop_=%d\n", cont_loop_);
    printf("CKeyInput::key_Loop: X.\n");
    return ret;
}

void CKeyInput::handle_key_event(char c)
{
    switch (c)
    {
    case KEYCODE_L:
        printf("LEFT\n");
        break;
    case KEYCODE_R:
        printf("RIGHT\n");
        break;
    case KEYCODE_U:
        printf("UP\n");
        break;
    case KEYCODE_D:
        printf("DOWN\n");
        break;
    default:
        printf("value: %c = 0x%02X = %d\n", c, c, c);
        break;
    }
}

void CKeyInput::exit_key_loop()
{
    printf("exit_key_loop\n");
    cont_loop_ = false;
}
