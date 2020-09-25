#ifndef __CMD_INPUT_H__
#define __CMD_INPUT_H__

#include <termios.h>


#define CMD_PROMPT    "=> "
#define MAX_CMD_LEN   2048

#define KEYCODE_UP    0x41
#define KEYCODE_DOWN  0x42
#define KEYCODE_RIGHT 0x43
#define KEYCODE_LEFT  0x44
#define KEYCODE_Q 0x71
#define KEYCODE_ENTER 0x0A
#define KEYCODE_SPACE 0x20
#define KEYCODE_ESC 0x1B

#define CmdInput    (*CCmdInput::inst())


class CCmdInput 
{
// Singleton Decl.
public:
    static CCmdInput* inst();
private:
    static CCmdInput* inst_; //the one and only object instance
protected:
    CCmdInput();
	virtual ~CCmdInput();

public:
    int enter_cmd_loop();
    void exit_cmd_loop();
    bool is_looping() { return looping_; }

protected:
    int read_cmd();
    void exec_cmd();
    void handle_key_input(char c, bool& cont_read);

private:
    struct termios org_settings_;
    bool looping_;

private:
    char cmd_buf_[MAX_CMD_LEN + 1];
    char *cmd_ptr;
};


#endif // __CMD_INPUT_H__
