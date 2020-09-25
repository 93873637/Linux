
#include <stdio.h>

#define STR(x) #x

#define PRINT_VAR(x) printf("my_" #x " = %d\n", my_##x)

///////////////////////////////////////////////////////////////////////////////

#define GET_ACTIVE_DEVICE         \
    char *pDevice = 0x1;       \
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

#define GET_SCA_DATA_INT(_type) \
    GET_SCA_DATA(_type);        \
    int data = R2_##_type;         \
    printf(#_type " = %d\n", data);

///////////////////////////////////////////////////////////////////////////////

void main()
{
    printf("main E...\n");

    int my_var_name = 123;
    PRINT_VAR(var_name);

    int R_Mode = 1;
    int R2_Mode = 1234;
    //GET_ACTIVE_DEVICE;
    //GET_SCA_DATA(Mode);
    GET_SCA_DATA_INT(Mode);

    printf("main X.\n");
}
