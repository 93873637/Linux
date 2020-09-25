#include <stdio.h>
#include <stdarg.h>
#include <memory.h>

class myclass
{
public:
    char b;
    char c;
    int e;
};

void test_class()
{
    printf("test_class\n");
    myclass *p = new myclass [10];
    printf("  p  = %x\n", p);
    printf("p+1  = %x\n", p+1);
    printf("p+2  = %x\n", p+2);
    printf("p[2] = %x\n", &p[2]);
    printf("++p  = %x\n", ++p);
    printf("p++  = %x\n", p++);
}

void test_char()
{
    char *p = new char [10];
    printf("  p  = %x\n", p);
    printf("p+1  = %x\n", p+1);
    printf("p+2  = %x\n", p+2);
    printf("p[2] = %x\n", &p[2]);
    printf("++p  = %x\n", ++p);
    printf("p++  = %x\n", p++);
}

void test_int()
{
    int *p = new int [10];
    printf("  p  = %x\n", p);
    printf("p+1  = %x\n", p+1);
    printf("p+2  = %x\n", p+2);
    printf("p[2] = %x\n", &p[2]);
    printf("++p  = %x\n", ++p);
    printf("p++  = %x\n", p++);
}

int main(int argc, char *argv[])
{
    test_class();    
    return 0;
}
