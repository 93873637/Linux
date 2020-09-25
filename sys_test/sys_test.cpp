#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    printf("PATH=%s\n", getenv("PATH"));
    printf("CM_PRODUCT=%s\n", getenv("CM_PRODUCT"));
    return 0;
}
