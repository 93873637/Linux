
#include <termio.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <conio.h>

void scanKeyboard()
{
    struct termios stored_settings;
    tcgetattr(0, &stored_settings);

    struct termios new_settings;
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0, &stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
    int in;
    while(1)
    {
        in = getch();
        printf("getchar: %d\n", in);
    }

    //restore original settings
    tcsetattr(0, TCSANOW, &stored_settings);
}

void get_password()
{
    #define LENGTH 8
    char passwd[LENGTH];
    struct termios initsettings;
    struct termios newsettings;
    printf("input your password\n");
    tcgetattr(fileno(stdin), &initsettings);
    newsettings = initsettings;
    newsettings.c_lflag &= ~ECHO;
    tcsetattr(fileno(stdin), TCSAFLUSH, &newsettings);
    fgets(passwd, LENGTH, stdin);
    tcsetattr(fileno(stdin), TCSANOW, &initsettings);
    fprintf(stdout, "your passwd is:");
    fprintf(stdout, "%s", passwd);
}

int main(int argc, char *argv[])
{
    scanKeyboard();
    return 0;
}
