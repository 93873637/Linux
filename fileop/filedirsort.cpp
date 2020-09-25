#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int custom_filter(const struct dirent *pDirent)
{
    return pDirent->d_type == DT_REG
        && strcmp(pDirent->d_name, ".") != 0 
        && strcmp(pDirent->d_name, "..") != 0
        ;
}

int main()
{
    const char *src_dir = "/home/cloud/proj/Camera_SuperNight/src/night_mode_daemon/images/IMG_20190126_223521_iso1476exp38";
    struct dirent **namelist;
    int n;
    int i;
    n = scandir(src_dir, &namelist, custom_filter, alphasort);
    if (n < 0)
    {
        perror("scandir");
    }
    else
    {
        for (i = 0; i < n; i++)
        {
            printf("%s\n", namelist[i]->d_name);
            free(namelist[i]);
        }
        free(namelist);
    }
}
