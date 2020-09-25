#ifndef __KEY_INPUT_H__
#define __KEY_INPUT_H__

#include <termios.h>

#define KEYBOARD_FD    0

#define KEYCODE_R 0x43
#define KEYCODE_L 0x44
#define KEYCODE_U 0x41
#define KEYCODE_D 0x42
#define KEYCODE_Q 0x71
#define KEYCODE_SPACE 0x20

#define KeyInput    (*CKeyInput::inst())


class CKeyInput 
{
// Singleton Decl.
public:
    static CKeyInput* inst();
private:
    static CKeyInput* inst_; //the one and only object instance
protected:
    CKeyInput();
	virtual ~CKeyInput();

public:
    int enter_key_loop();
    void exit_key_loop();
    bool key_loop() { return key_loop_; }
protected:
    void handle_key_event(char c);

private:
    struct termios org_settings_;
    bool key_loop_;
};


#endif // __KEY_INPUT_H__
