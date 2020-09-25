#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define KEYBOARD_FD    0

#define KEYCODE_R 0x43
#define KEYCODE_L 0x44
#define KEYCODE_U 0x41
#define KEYCODE_D 0x42
#define KEYCODE_Q 0x71

struct termios org_settings;

void quit(int sig)
{
    printf("restore original settings...\n");
    (void)sig;
    tcsetattr(KEYBOARD_FD, TCSANOW, &org_settings);
    printf("quit\n");
    exit(0);
}

void keyLoop()
{
    signal(SIGINT, quit);
    
    // get the console in new_settings mode
    tcgetattr(KEYBOARD_FD, &org_settings);

    struct termios new_settings;
    memcpy(&new_settings, &org_settings, sizeof(struct termios));
    new_settings.c_lflag &=~(ICANON | ECHO);
    new_settings.c_cc[VEOL] = 1;
    new_settings.c_cc[VEOF] = 2;
    tcsetattr(KEYBOARD_FD, TCSANOW, &new_settings);

    char c;
    for (;;)
    {
        if (read(KEYBOARD_FD, &c, 1) < 0)
        {
            perror("read():");
            exit(-1);
        }

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
}

int main(int argc, char** argv)
{
    
    puts("Reading from keyboard");
    puts("---------------------------");
    puts("Use arrow keys to move the robot.");
    puts("otherwise the key values will be printed");

    keyLoop();

    return(0);
}
