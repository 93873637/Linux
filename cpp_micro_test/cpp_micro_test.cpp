
#include <stdio.h>

class MyClass
{
public:
    int Mode;
};

#define STR(x) #x

#define PRINT_VAR(x) printf("my_" #x " = %d\n", my_##x)

///////////////////////////////////////////////////////////////////////////////

#define GET_ACTIVE_DEVICE         \
    char *pDevice = "aaa";       \
    if (pDevice == 0)             \
    {                             \
        printf("device null.\n"); \
        return;                   \
    }

#define GET_SCA_DATA(_type)                       \
    GET_ACTIVE_DEVICE;                            \
    if (!R_##_type)                               \
    {                                             \
        printf("request " #_type " failed\n"); \
        return;                                   \
    }

#define GET_SCA_DATA_INT(_type, _type_t) \
    GET_SCA_DATA(_type);       \
    _type_t data = pc->_type;\
    printf("#####################"#_type" = %d\n", data);

void test_micro()
{
    MyClass *pc = new MyClass();
    pc->Mode = 56789;

    int R_Mode = 1;
    int R2_Mode = 1234;
    //GET_ACTIVE_DEVICE;
    //GET_SCA_DATA(Mode);
    GET_SCA_DATA_INT(Mode, int);
}
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) 
{
    printf("main E...\n");

    int my_var_name = 123;
    PRINT_VAR(var_name);

    test_micro();

    printf("main X.\n");
    return 0;
}
