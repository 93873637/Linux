#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "CmdInput.h"

using namespace std;

#define CLEAR_CMD_BUF                          \
    {                                          \
        memset(cmd_buf_, 0, sizeof(cmd_buf_)); \
        cmd_ptr = &cmd_buf_[0];                \
    }
#define CMD_LEN (cmd_ptr - (&cmd_buf_[0]))

static string::iterator s_list_cmd;
static vector<string> s_input_list;
ssize_t tread(int fd, void *buf, size_t nbytes, unsigned int timeout);

//////////////////////////////////////////////////////////////////////////
// Singleton Decl.

CCmdInput *CCmdInput::inst_ = NULL;
CCmdInput *CCmdInput::inst()
{
    if (CCmdInput::inst_ == NULL)
    {
        CCmdInput::inst_ = new CCmdInput();
    }
    return CCmdInput::inst_;
}

CCmdInput::CCmdInput()
    : looping_(false)
{
    CLEAR_CMD_BUF;
}

CCmdInput::~CCmdInput()
{
}

// Singleton Decl.
//////////////////////////////////////////////////////////////////////////

static void on_sigint(int sig)
{
    if (CmdInput.is_looping())
    {
        CmdInput.exit_cmd_loop();
    }
    pthread_exit(0);
}

int CCmdInput::enter_cmd_loop()
{
    printf("cmd_loop: E...\n");
    signal(SIGINT, on_sigint); // check ctrl+c signal on cmd mode

    printf("save original settings...\n");
    tcgetattr(STDIN_FILENO, &org_settings_);

    // struct termios new_settings;
    // memcpy(&new_settings, &org_settings_, sizeof(struct termios));
    // new_settings.c_lflag &= ~(ICANON | ECHO);
    // new_settings.c_cc[VEOL] = 1;
    // new_settings.c_cc[VEOF] = 2;
    // tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
    tcsetattr(STDIN_FILENO, TCSANOW, &org_settings_);

    int ret = 0;
    looping_ = true;
    while (looping_)
    {
        printf("%s", CMD_PROMPT);
        fflush(stdout);

        ret = read_cmd();
        if (ret < 0)
        {
            printf("read cmd error %d\n", ret);
            break;
        }
        else
        {
            exec_cmd();
        }

        printf("\n");
    }

    exit_cmd_loop();
    printf("cmd_loop: X.\n");
    return ret;
}

int CCmdInput::read_cmd()
{
    CLEAR_CMD_BUF;
    int ret;
    char c;
    bool cont_read = true;
    while (cont_read)
    {
        ret = tread(STDIN_FILENO, &c, 1, 1);
        if (ret < 0)
        {
            printf("tread error %d\n", ret);
            return ret;
        }
        else if (ret == 0)
        {
            //printf("tread timeout\n");
            continue;
        }
        else
        {
            handle_key_input(c, cont_read);
        }
    }
    return 0;
}

void CCmdInput::handle_key_input(char c, bool &cont_read)
{
    cont_read = true;
    switch (c)
    {
    case KEYCODE_UP:
        //printf("KEYCODE_UP\n");
        // if (s_list_cmd != s_input_list::begin())
        // {
        //     s_list_cmd --;

        //     //####@:
        // }
        break;
    case KEYCODE_DOWN:
        //printf("KEYCODE_DOWN\n");
        {

        }
        break;

    // case 0x08:  //KEYCODE_BACKSPACE
    //     printf("KEYCODE_BACKSPACE\n");
    //     {
    //     //    printf("\b");
    //     //    fflush(stdout);
    //     }
    //     break;

    // case 0x7F:  //KEYCODE_DELETE
    //     printf("KEYCODE_DELETE\n");
    //     {
    //         // printf("\b");
    //         // fflush(stdout);
    //     }
    //     break;

    case KEYCODE_RIGHT:
        //printf("KEYCODE_RIGHT\n");
        {
            //printf("ff\b");
            putchar(c);
        }
        break;

    case KEYCODE_LEFT:
        //printf("KEYCODE_LEFT\n");
        {
    //printf("ff\b");
        putchar(c);
        }
        break;

    case KEYCODE_ENTER:
        //printf("KEYCODE_ENTER\n");
        cont_read = false;
        return;

    default:
        printf("get key input: '%c' = 0x%02X = %d\n", c, c, c);
        // if (CMD_LEN < MAX_CMD_LEN)
        // {
        //     *cmd_ptr++ = c;
        //     printf("%c", c);
        //     fflush(stdout);
        // }
        break;
    }
}

/*
 * tread:
 * "Timed" read - timeout specifies the # of seconds to wait before
 * giving up (5th argument to select controls how long to wait for
 * data to be readable).  Returns # of bytes read or -1 on error.
 */
ssize_t tread(int fd, void *buf, size_t nbytes, unsigned int timeout)
{
    int nfds;
    fd_set readfds;
    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    nfds = select(fd + 1, &readfds, NULL, NULL, &tv);
    if (nfds < 0)
    {
        printf("select error %d\n", nfds);
        return -1;
    }
    else if (nfds == 0)
    {
        //printf("select timeout\n");
        return 0;
    }
    else
    {
        return read(fd, buf, nbytes);
    }
}

static string& trim(string& s)
{
    if (s.empty())
    {
        return s;
    }
    else
    {
        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
        return s;
    }
}

void CCmdInput::exec_cmd()
{
    if (CMD_LEN > 0)
    {
        string cmd_str = cmd_buf_;
        if (!trim(cmd_str).empty())
        {
            printf("\n");
            printf("TODO: exec_cmd: \"%s\"", cmd_str.c_str());
            //####@:

            if (s_input_list.back() != cmd_str)
            {
                s_input_list.push_back(cmd_str);
                //####@: s_list_index = s_input_list.end();
            }
        }
    }
}

void CCmdInput::exit_cmd_loop()
{
    printf("exit_cmd_loop\n");
    //printf("restore original settings...\n");
    //tcsetattr(STDIN_FILENO, TCSANOW, &org_settings_);
    looping_ = false;
}
