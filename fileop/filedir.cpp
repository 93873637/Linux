#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <map>
#include <string>

using namespace std;
 
/***** Global Variables *****/
char dir[256] = "/home/cloud/proj/Camera_SuperNight/res/images/1216/a2h/iso20000_exp250";
int const MAX_STR_LEN = 200;
 
 //void load_image_files(std::vector<std::string> &image_files);
    
/* Show all files under image_path , do not show directories ! */
void load_image_files(const char *image_path, std::vector<std::string> &image_files)
{
    if (image_path == NULL)
    {
        cout << "image_path null" << endl;
        return;
    }
 
    struct stat s;
    lstat(image_path, &s);
    if (!S_ISDIR(s.st_mode))
    {
        cout << "invalid image path" << endl;
        return;
    }
    
    struct dirent *file;    // return value for readdir()
     DIR *dir;               // return value for opendir()
    dir = opendir(image_path);
    if (dir == NULL)
    {
        cout << "Can not open dir: " << image_path << endl;
        return;
    }
    
    /* read all the files in the dir ~ */
    while((file = readdir(dir)) != NULL)
    {
        // skip "." and ".."
        if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
            continue;
        std::string image_file = image_path;
        image_files.push_back(image_file + file->d_name);
    }
} 
 
int main()
{
    std::vector<std::string> image_files;

    // 测试
    load_image_files(dir, image_files);
    
    for (size_t i =0; i < image_files.size(); i ++) {
        cout << image_files[i] << endl;
    }

    return 0;
}
