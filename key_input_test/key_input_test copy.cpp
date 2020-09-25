#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    int keys_fd;
    char ret[2];
    struct input_event t;

    keys_fd = open("/dev/input/event4", O_RDONLY);
    if (keys_fd <= 0)
    {
        printf("open /dev/input/event4 device error!\n");
        return 0;
    }

    while (1)
    {
        if (read(keys_fd, &t, sizeof(t)) == sizeof(t))
        {
            if (t.type == EV_KEY)
                if (t.value == 0 || t.value == 1)
                {
                    printf("key %d %s\n", t.code,
                        (t.value) ? "Pressed" : "Released");
                    if (t.code==KEY_ESC)
                        break;
                }
        }
    }
    close(keys_fd);

    return 0;
}

// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>  
// #include <unistd.h>
// #include <fcntl.h>
// #include<sys/types.h>
// #include<sys/stat.h>
// #include <errno.h>
// #include <termios.h>

// static struct termios initial_settings, new_settings;
// static int peek_character = -1;
// void init_keyboard(void);
// void close_keyboard(void);
// int kbhit(void);
// int readch(void); 
// void init_keyboard()
// {
// 	tcgetattr(0,&initial_settings);
// 	new_settings = initial_settings;
// 	new_settings.c_lflag |= ICANON;
// 	new_settings.c_lflag |= ECHO;
// 	new_settings.c_lflag |= ISIG;
// 	new_settings.c_cc[VMIN] = 1;
// 	new_settings.c_cc[VTIME] = 0;
// 	tcsetattr(0, TCSANOW, &new_settings);
// }

// void close_keyboard()
// {
// 	tcsetattr(0, TCSANOW, &initial_settings);
// }

// int kbhit()
// {
// 	unsigned char ch;
// 	int nread;

// 	if (peek_character != -1) return 1;
// 	new_settings.c_cc[VMIN]=0;
// 	tcsetattr(0, TCSANOW, &new_settings);
// 	nread = read(0,&ch,1);
// 	new_settings.c_cc[VMIN]=1;
// 	tcsetattr(0, TCSANOW, &new_settings);
// 	if(nread == 1) 
// 	{
// 		peek_character = ch;
// 		return 1;
// 	}
// 	return 0;
// }

// int readch()
// {
// 	char ch;

// 	if(peek_character != -1) 
// 	{
// 		ch = peek_character;
// 		peek_character = -1;
// 		return ch;
// 	}
// 	read(0,&ch,1);
// 	return ch;
// }

// int main()
// {
// 	init_keyboard();
// 	while(1)
// 	{
// 		kbhit();
// 		printf("\n%d\n", readch());
// 	}
// 	close_keyboard();
// 	return 0;
// }

// // #include <termio.h>
// // #include <stdio.h>


// // int scanKeyboard()
// // {
// //     struct termios new_settings;
// //     struct termios stored_settings;
// //     tcgetattr(0, &stored_settings);
// //     new_settings = stored_settings;
// //     new_settings.c_lflag &= (~ICANON);
// //     new_settings.c_cc[VTIME] = 0;
// //     tcgetattr(0, &stored_settings);
// //     new_settings.c_cc[VMIN] = 1;
// //     tcsetattr(0, TCSANOW, &new_settings);

// //     int in;
// //     while(1)
// //     {
// //         in = getchar();
// //         printf("getchar: %d\n", in);
// //         if (in == 27)
// //         {
// //             break;
// //         }
// //     }

// //     tcsetattr(0, TCSANOW, &stored_settings);
// //     return in;
// // }

// // int main(int argc, char *argv[])
// // {
// //     scanKeyboard();
// // }
