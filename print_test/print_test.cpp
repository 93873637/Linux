#include <stdio.h>
#include <stdarg.h>
#include <memory.h>

#define MAXLEN 10

int mon_log(const char *format, ...)
{
    char str_tmp[MAXLEN];
    memset((void*)str_tmp, '\xff', MAXLEN);
    for (int i = 0; i < MAXLEN; i++)
    {
        printf("%02X  ", (unsigned char)str_tmp[i]);
    }
    printf("\n");
    
    va_list vArgList;
    va_start(vArgList, format);
    int ret = vsnprintf(str_tmp, MAXLEN, format, vArgList);
    va_end(vArgList);

    printf("str_tmp=\"%s\"\n", str_tmp);
    for (int j = 0; j < MAXLEN; j++)
    {
        printf("%02X  ", (unsigned char)str_tmp[j]);
    }
    printf("\n");
    return ret;
}

void test_log_msg(const char *msg)
{
    printf("\n");
    printf("test log msg: \"%s\"\n", msg);
    int ret = mon_log(msg);
    printf("ret = %d\n", ret);
}

void printf_hex(char *buf, int ret)
{
	if (ret <= 0)
		return;
	
	while (ret--) {
        char *p=buf;
	    printf("%x=%02x, ", p, *p);
        buf ++;
    }

	printf("\r\n");
}

void print_hex(const char *buf, int size)
{
	int i = 0;

	//printf("str: \n");

	for(i=0; i<size; i++) {
		printf("%x=%02x, ", &(buf[i]), buf[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
    // test_log_msg("12345");
    // test_log_msg("12345\n");
    // test_log_msg("123456789");
    // test_log_msg("1234567890");
    // test_log_msg("1234567890\n");
    // test_log_msg("123456789012345");

    char a[256];
    for (int i=0; i<256; i++)
        a[i] = '0' + i;
    
    printf("1: a=%x\n", a);
    printf_hex(a, 9);
    print_hex(a, 9);
    printf("2: a=%x\n", a);
    return 0;
}
