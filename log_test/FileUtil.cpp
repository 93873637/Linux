/******************************************************************************
 * This file is part of libemb.
 *
 * libemb is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libemb is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libemb.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Project: Embedme
 * Author : FergusZeng
 * Email  : cblock@126.com
 * git	  : https://git.oschina.net/cblock/embedme
 * Copyright 2014~2020 @ ShenZhen ,China
*******************************************************************************/
#include "FileUtil.h"
#include "ComUtil.h"
#include "LogUtil.h"
#include "ProcUtil.h"
#include "StrUtil.h"

#include <algorithm>
#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace libemb
{
/**
 *  \brief  File构造函数
 *  \param  none
 *  \note   none
 */
File::File() : m_fp(NULL),
               m_name(""),
               m_mmaddr(NULL)
{
}

File::~File()
{
    this->close();
}

/**
 *  \brief  判断文件是否存在
 *  \param  filePath 文件名
 *  \return 文件存在返回true,否则返回false
 *  \note   只有name是文件名称时才返回true,如果是目录名则返回false
 */
bool File::exists(const char *filePath)
{
    if (0 == access(filePath, F_OK))
    {
        struct stat fileStat;
        if (0 != lstat(filePath, &fileStat))
        {
            return false;
        }
        if (S_ISDIR(fileStat.st_mode))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    return false;
}
/**
 *  \brief  删除文件
 *  \param  fileName 文件名
 *  \return 成功返回true,失败返回false
 *  \note   none
 */
bool File::deleteFile(const char *fileName)
{
    if (remove(fileName) != 0)
    {
        PRINT_E("remove file failed, error: %s\n", fileName, ERR_STR); // NOTE: using PRINT_* instead of LOG_* in case recursive calling
        return false;
    }
    return true;
}
/**
 *  \brief  获取内容大小
 *  \param  filePath 文件名或文件夹名
 *  \return 返回文件的大小,以byte为单位
 *  \note   none
 */
int File::getContentSize(const char *fileName)
{
    struct stat statbuf;
    if (0 == lstat(fileName, &statbuf))
    {
        int size = statbuf.st_size;
        return size;
    }
    return 0;
}

/**
 *  \brief  文件是否已经打开
 *  \param  void
 *  \return 文件已打开返回true,否则返回false
 *  \note   none
 */
bool File::isOpen()
{
    if (m_fp == NULL)
    {
        return false;
    }
    return true;
}
/**
 *  \brief  打开文件
 *  \param  fileName 文件全名(包含路径,如:/etc/init.d/rcS)
 *  \param  ioMode 打开模式IO_MODE_E
 *  \return 文件打开成功返回true,失败返回false
 *  \note   none
 */
bool File::open(const char *fileName, int ioMode)
{
    std::string modestr;
    if (fileName == NULL)
    {
        PRINT_E("file name is NULL.\n");
        return false;
    }

    switch (ioMode)
    {
    case IO_MODE_RD_ONLY:
        modestr = "rb";
        break;
    case IO_MODE_WR_ONLY:
    case IO_MODE_RDWR_ONLY:
        modestr = "rb+";
        break;
    case IO_MODE_REWR_ORNEW:
        modestr = "wb";
        break;
    case IO_MODE_RDWR_ORNEW:
        modestr = "wb+";
        break;
    case IO_MODE_APPEND_ORNEW:
        modestr = "ab";
        break;
    case IO_MODE_RDAPPEND_ORNEW:
        modestr = "ab+";
        break;
    default:
        PRINT_E("Unsupport mode(%d)\n", ioMode);
        return false;
    }

    if (m_fp != NULL)
    {
        fclose(m_fp);
        m_fp = NULL;
    }

    m_fp = fopen(fileName, CSTR(modestr));
    if (NULL == m_fp)
    {
        PRINT_E("open file:%s, error:%s.\n", fileName, ERR_STR);
        return false;
    }
    m_fd = fileno(m_fp);
    m_name = fileName;
    return true;
}

/**
 *  \brief  关闭文件
 *  \param  void
 *  \return 文件关闭成功返回true,失败返回false
 *  \note   none
 */
bool File::close()
{
    if (m_fp != NULL)
    {
        fclose(m_fp);
        m_fp = NULL;
    }
    return true;
}

/**
 *  \brief  读取数据
 *  \param  buf 数据缓存
 *  \param  count 要读取的长度
 *  \return 成功返回读取的数据长度,失败返回STATUS_ERROR
 *  \note   none
 */
int File::readData(char *buf, int len)
{
    if (NULL == m_fp)
    {
        PRINT_E("File not open.\n");
        return STATUS_ERROR;
    }
    if (NULL == buf || len <= 0)
    {
        PRINT_E("parameter error:buf=0x%x,count=%d.\n", buf, len);
        return STATUS_ERROR;
    }
    return fread(buf, 1, len, m_fp);
}

/**
 *  \brief  写入数据
 *  \param  buf 数据缓存
 *  \param  count 要写入的长度
 *  \return 成功返回写入的数据长度,失败返回STATUS_ERROR
 *  \note   none
 */
int File::writeData(const char *buf, int len)
{
    int ret;
    if (NULL == m_fp)
    {
        PRINT_E("File not open.\n");
        return STATUS_ERROR;
    }
    if (NULL == buf || len <= 0)
    {
        PRINT_E("parameter error.\n");
        return STATUS_ERROR;
    }
    ret = fwrite(buf, 1, len, m_fp);
    fflush(m_fp);
    return ret;
}

/**
 *  \brief  将文件映射到内存
 *  \return 映射成功返回地址,失败返回NULL
 *  \note   在使用mapMemery时,请不要使用readData,writeData这些接口
 */
void *File::mapMemory()
{
    if (m_mmaddr == NULL)
    {
        m_mmaddr = mmap(NULL, getSize(), PROT_READ | PROT_WRITE, MAP_SHARED, fileno(m_fp), 0);
        if (MAP_FAILED == m_mmaddr)
        {
            PRINT_E("file[%s] map memory error:%s!\n", CSTR(m_name), ERR_STR);
            return NULL;
        }
    }
    return m_mmaddr;
}

/**
 *  \brief  解除文件内存映射
 *  \return 成功返回STATUS_OK,失败返回STATUS_OK
 *  \note   在使用mapMemery时,请不要使用readData,writeData这些接口
 */
int File::unmapMemory()
{
    if (m_mmaddr != NULL)
    {
        if (-1 == munmap(m_mmaddr, getSize()))
        {
            PRINT_E("file[%s] unmap memory error:%s!\n", CSTR(m_name), ERR_STR);
            return STATUS_ERROR;
        }
        m_mmaddr = NULL;
    }
    return STATUS_OK;
}

/**
 *  \brief  读取一行数据
 *  \param  lineStr 该行字符串
 *  \return 成功返回读取的数据长度,失败返回STATUS_ERROR
 *  \note   none
 */
int File::readLine(std::string &lineStr)
{
    int ret;
    if (NULL == m_fp)
    {
        PRINT_E("File not open.\n");
        return STATUS_ERROR;
    }
    lineStr = "";
    while (1)
    {
        char buf[2] = {0};
        ret = readData(buf, 1);
        if (STATUS_ERROR == ret)
        {
            return STATUS_ERROR;
        }
        else if (0 == ret)
        {
            if (isEnd())
            {
                return lineStr.size();
            }
            else
            {
                return STATUS_ERROR;
            }
        }
        else
        {
            lineStr += buf;
            if (std::string::npos != lineStr.find("\r\n"))
            {
                lineStr = lineStr.substr(0, lineStr.size() - 2);
                return lineStr.size();
            }
            else if (std::string::npos != lineStr.find("\n"))
            {
                lineStr = lineStr.substr(0, lineStr.size() - 1);
                return lineStr.size();
            }
        }
    }
}

/**
 *  \brief  获取文件大小
 *  \param  void
 *  \return 成功返回文件大小,失败返回STATUS_ERROR
 *  \note   none
 */
int File::getSize()
{
    int fileSize, pos;
    if (m_fp == NULL)
    {
        PRINT_E("get size failed, fp null\n");
        return STATUS_ERROR;
    }
    pos = ftell(m_fp); /* 保存当前读写位置 */
    if (fseek(m_fp, 0, SEEK_END) < 0)
    {
        PRINT_E("get size failed, fseek error\n");
        return STATUS_ERROR;
    }
    fileSize = ftell(m_fp);
    if (fileSize < 0)
    {
        PRINT_E("get size failed, ftell error\n");
        fileSize = STATUS_ERROR;
    }
    fseek(m_fp, pos, SEEK_SET); /* 恢复当前读写位置 */
    return fileSize;
}

/**
 *  \brief  获取文件名称
 *  \param  void
 *  \return 返回当前文件名称
 *  \note   none
 */
const std::string& File::getName()
{
    return m_name;
}

/**
 *  \brief  判断是否已经到文件尾部
 *  \param  void
 *  \return 是返回true,否返回false
 *  \note   none
 */
bool File::isEnd()
{
    int pos = getPos();
    if (pos == STATUS_ERROR || pos != getSize())
    {
        return false;
    }
    return true;
}

/**
 *  \brief  获取当前读写位置
 *  \param  void
 *  \return 成功返回当前读写位置,失败返回STATUS_ERROR
 *  \note   none
 */
int File::getPos()
{
    int pos;
    if (NULL == m_fp)
    {
        PRINT_E("File not open.\n");
        return STATUS_ERROR;
    }
    pos = ftell(m_fp);
    if (pos < 0)
    {
        return STATUS_ERROR;
    }
    else
    {
        return pos;
    }
}

/**
 *  \brief  设置当前读写位置
 *  \param  void
 *  \return 成功返回OK,失败返回STATUS_ERROR
 *  \note   none
 */
int File::setPos(int pos)
{
    if (NULL == m_fp)
    {
        PRINT_E("File not open.\n");
        return STATUS_ERROR;
    }
    if (0 == fseek(m_fp, pos, SEEK_SET))
    {
        return STATUS_OK;
    }
    else
    {
        return STATUS_ERROR;
    }
}

FilePath::FilePath(const char *filePath) : m_filePath(filePath),
                                           m_dir(NULL),
                                           m_base(NULL)
{
}
FilePath::~FilePath()
{
    if (m_dir != NULL)
    {
        free(m_dir);
        m_dir = NULL;
    }
    if (m_base != NULL)
    {
        free(m_base);
        m_base = NULL;
    }
}

std::string FilePath::dirName()
{
    if (m_dir == NULL)
    {
        m_dir = strdup(CSTR(m_filePath));
    }
    return std::string(dirname(m_dir));
}

std::string FilePath::baseName()
{
    if (m_base == NULL)
    {
        m_base = strdup(CSTR(m_filePath));
    }
    return std::string(basename(m_base));
}

bool FilePath::createLink(const char *filePath, const char *linkPath, bool hard)
{
    if (filePath == NULL || linkPath == NULL)
    {
        return false;
    }
    if (hard)
    {
        if (link(filePath, linkPath) != 0)
        {
            PRINT_D("create hard link error:%s\n", ERR_STR);
            return false;
        }
    }
    else
    {
        if (symlink(filePath, linkPath) != 0)
        {
            PRINT_D("create soft link error:%s\n", ERR_STR);
            return false;
        }
    }
    return true;
}

Directory::Directory() : m_currentPath(".")
{
}

Directory::~Directory()
{
}

/**
 *  \brief  判断目录是否为空
 *  \param  name 目录全名(如:/usr/share)
 *  \return 目录为空或不存在返回true,不为空则返回false
 *  \note   none
 */
bool Directory::isEmpty(const char *name)
{
    DIR *dirp = NULL;
    struct dirent *dp = NULL;
    int num = 0;
    dirp = opendir(name);
    if (NULL == dirp)
    {
        //PRINT_E("Directory::isDirEmpty open dir:%s error(%s).\n",name,ERR_STR);
        return true;
    }
    while ((dp = readdir(dirp)) != NULL)
    {
        if (strcmp(dp->d_name, ".") == 0 ||
            strcmp(dp->d_name, "..") == 0)
        {
            continue;
        }
        else
        {
            num++;
            break;
        }
    }
    closedir(dirp);
    if (num > 0)
    {
        return false;
    }
    return true;
}

/**
 *  \brief  判断目录是否存在
 *  \param  name 目录全名(如:/usr/share)
 *  \return 目录存在返回true,不存在则返回false
 *  \note   只有name是目录名称时才返回true,如果是文件名则返回false
 */
bool Directory::exists(const char *name)
{
    if (name == NULL)
    {
        return false;
    }
    if (0 == access(name, F_OK))
    {
        struct stat fileStat;
        if (0 != lstat(name, &fileStat))
        {
            return false;
        }
        else
        {
            if (S_ISDIR(fileStat.st_mode))
            {
                return true;
            }
            if (S_ISLNK(fileStat.st_mode))
            {
                DIR *dirp = opendir(name);
                if (dirp != NULL)
                {
                    closedir(dirp);
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 *  \brief  获取文件夹内容大小
 *  \param  name 文件夹名
 *  \return 返回文件夹的大小，以byte为单位
 *  \note   目前该函数会计算硬链接文件的大小,这样得到的结果会比实际的大,
 *          因此该函数不适用于存在硬链接的场合.
 */
int Directory::getContentSize(const char *name)
{
    if (Directory::exists(name))
    {
        int sum = 0;
        Directory dir;
        std::string path(name);
        dir.enter(path);
        std::vector<std::string> fileList = dir.listAll();

        int fileNum = fileList.size();
        for (int i = 0; i < fileNum; i++)
        {
            if (fileList[i] == "." ||
                fileList[i] == "..")
            {
                continue;
            }
            std::string fileName = path + "/";
            fileName.append(fileList[i]);
            if (File::exists(CSTR(fileName)))
            {
                sum += File::getContentSize(CSTR(fileName));
            }
            if (Directory::exists(CSTR(fileName)))
            {
                sum += File::getContentSize(CSTR(fileName));
                sum += Directory::getContentSize(CSTR(fileName));
            }
        }
        return sum;
    }
    else
    {
        PRINT_E("\"%s\" not exist!\n", name);
        return 0;
    }
}

/**
 *  \brief  创建文件夹
 *  \param  name 文件夹名
 *  \param  recursice 是否递归
 *  \return 成功返回true,失败返回false
 *  \note   none
 */
bool Directory::createDir(const char *name, bool recursive)
{
    if (name == NULL)
        return false;
    if (recursive)
    {
        vector<string> dirNames = StrUtil::splitString(name, "/", true);
        if (!dirNames.empty())
        {
            string dirPath = "";
            for (size_t i = 0; i < dirNames.size(); i++)
            {
                if (dirNames[i] == "/")
                {
                    if (i == 0)
                        dirPath += "/";
                }
                else
                {
                    dirPath += dirNames[i];
                    dirPath += "/";
                    if (!Directory::exists(CSTR(dirPath)))
                    {
                        if (mkdir(CSTR(dirPath), 0644) != 0)
                        {
                            PRINT_E("Directory::createDir(%s) error:%s.\n", CSTR(dirPath), ERR_STR);
                            return false;
                        }
                    }
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (mkdir(name, 0644) != 0)
        {
            PRINT_E("Directory::createDir(%s) error:%s.\n", name, ERR_STR);
            return false;
        }
        else
        {
            return true;
        }
    }
}
/**
 *  \brief  删除文件夹
 *  \param  name 文件夹名
 *  \param  recursice 是否递归
 *  \return 成功返回true,失败返回false
 *  \note   none
 */
bool Directory::deleteDir(const char *name, bool recursive)
{
    if (name == NULL)
        return false;
    if (recursive)
    {
        vector<string> dirNames = StrUtil::splitString(name, "/", true);
        if (!dirNames.empty())
        {
            string dirPath = "";
            for (size_t i = 0; i < dirNames.size(); i++)
            {
                if (dirNames[i] == "/")
                {
                    if (i == 0 || (i == (dirNames.size() - 1)))
                    {
                        dirPath += "/";
                    }
                }
                else
                {
                    dirPath += dirNames[i];
                    dirPath += "/";
                }
            }
            if (Directory::exists(CSTR(dirPath)))
            {
                if (!Directory::isEmpty(CSTR(dirPath)))
                { /* 先删除该目录下的所有子目录和文件 */
                    Directory dir;
                    dir.enter(CSTR(dirPath));
                    vector<string> allFiles = dir.listAll();
                    for (size_t i = 0; i < allFiles.size(); i++)
                    {
                        if (allFiles[i] == "." || allFiles[i] == "..")
                        {
                            continue;
                        }
                        else
                        {
                            string fileName = dirPath + allFiles[i];
                            if (Directory::exists(CSTR(fileName)))
                            {
                                if (!deleteDir(CSTR(fileName), true))
                                {
                                    PRINT_E("Directory::deleteDir,del1 dir(%s) error:%s.\n", CSTR(fileName), ERR_STR);
                                    return false;
                                }
                            }
                            else
                            {
                                if (!File::deleteFile(CSTR(fileName)))
                                {
                                    PRINT_E("Directory::deleteDir,del1 file(%s) error:%s.\n", CSTR(fileName), ERR_STR);
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
            return deleteDir(CSTR(dirPath)); /* 删除目录 */
        }
        return false;
    }
    else
    {
        if (remove(name) != 0)
        {
            PRINT_E("Directory::deleteDir(%s) error:%s.\n", name, ERR_STR);
            return false;
        }
        return true;
    }
}

/**
 *  \brief  进入目录
 *  \param  path 目录全名(如:/usr/share)
 *  \return 成功返回true，失败返回false
 *  \note   none
 */
bool Directory::enter(const string &path)
{
    string resolved;
    if (path.empty())
    {
        return false;
    }
    if (path[0] != '/')
    {
        resolved = m_currentPath + "/" + path;
    }
    else
    {
        resolved = path;
    }
    DIR *dirp = opendir(CSTR(resolved));
    if (NULL == dirp)
    {
        return false;
    }
    closedir(dirp);
    char buf[1024] = {0};
    if (NULL == realpath(CSTR(resolved), buf))
    {
        return false;
    }
    m_currentPath = string(buf);
    return true;
}

/**
 *  \brief  获取当前路径
 *  \param  none
 *  \return 返回绝对路径
 *  \note   none
 */
string Directory::current()
{
    return m_currentPath;
}

/**
 *  \brief  获取目录下所有文件
 *  \param  none
 *  \return 返回所有文件名
 *  \note   none
 */
vector<string> Directory::listAll()
{
    vector<string> fileVector;
    std::string fileName;
    DIR *dirp = opendir(CSTR(m_currentPath));
    if (NULL == dirp)
    {
        return fileVector;
    }
#if 0
    struct dirent *dp1 = (struct dirent*)malloc(sizeof(struct dirent));
    struct dirent *dp2 = (struct dirent*)malloc(sizeof(struct dirent));
    while ((readdir_r(dirp,dp1,&dp2))!=0)
    {
        fileName = string(dp2->d_name);
        fileVector.push_back(fileName);
    }
    closedir(dirp);
    free(dp1);
    free(dp2);
#else
    struct dirent *dp;
    while ((dp = readdir(dirp)) != NULL)
    {
        fileName = std::string(dp->d_name);
        fileVector.push_back(fileName);
    }
    closedir(dirp);
#endif
    sort(fileVector.begin(), fileVector.end());
    return fileVector;
}

} // namespace libemb
