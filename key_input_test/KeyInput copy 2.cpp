#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "KeyInput.h"
//###@: #include "//###@:SCAManager.h"


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
    : key_loop_(false)
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
ssize_t tread(int fd, void *buf, size_t nbytes, unsigned int timout)
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

int CKeyInput::enter_key_loop()
{
    printf("CKeyInput::key_Loop: E...\n");
    tcgetattr(KEYBOARD_FD, &org_settings_);

    struct termios new_settings;
    memcpy(&new_settings, &org_settings_, sizeof(struct termios));
    new_settings.c_lflag &=~(ICANON | ECHO);
    new_settings.c_cc[VEOL] = 1;
    new_settings.c_cc[VEOF] = 2;
    tcsetattr(KEYBOARD_FD, TCSANOW, &new_settings);

    int ret;
    char c;
    key_loop_ = true;
    while (key_loop_)
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
    
    printf("key_loop_ = %d, ret = %d\n", key_loop_, ret);
    key_loop_ = false;
    printf("CKeyInput::key_Loop: X.\n");
    return ret;
}

void CKeyInput::handle_key_event(char c)
{
    switch (c)
    {
    case KEYCODE_L:
        printf("LEFT\n");
        //###@:SCAManager.steer_left();
        break;
    case KEYCODE_R:
        printf("RIGHT\n");
        //###@:SCAManager.steer_right();
        break;
    case KEYCODE_U:
        printf("UP\n");
        //###@:SCAManager.steer_ahead();
        break;
    case KEYCODE_D:
        printf("DOWN\n");
        //###@:SCAManager.steer_back();
        break;
    case KEYCODE_SPACE:
        //###@:SCAManager.steer_stop();
        break;
    case 'h':
    case 'H':
        //###@:SCAManager.steer_straight();
        break;
    //case 27:  // 'ESC'  // left/right/up/down arrow also send 27/91, so don't using esc key as exit
    case 'q':
    case 'Q':
        exit_key_loop();
        break;
    default:
        printf("value: %c = 0x%02X = %d\n", c, c, c);
        break;
    }
}

void CKeyInput::exit_key_loop()
{
    printf("exit_key_loop\n");
    //###@:SCAManager.stop_all_devices();
    key_loop_ = false;
}
