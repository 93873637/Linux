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
#ifndef __FILE_DIR_H__
#define __FILE_DIR_H__

#include "BaseType.h"
#include "IODevice.h"

namespace libemb
{
    /**
 *  \file   FileUtil.h   
 *  \class  File
 *  \brief  文件类
 */
    class File : public IODevice
    {
    public:
        File();
        virtual ~File();
        bool isOpen();
        static bool exists(const char *filePath);
        static int getContentSize(const char *fileName);
        static bool deleteFile(const char *fileName);

        /* 以下方法继承自IODevice,请参考IODevice定义. */
        virtual bool open(const char *fileName, int ioMode);
        virtual bool close();
        virtual int readData(char *buf, int len);
        virtual int writeData(const char *buf, int len);

        void *mapMemory();
        int unmapMemory();
        int readLine(std::string &lineStr);
        int getSize();
        const std::string& getName();
        bool isEnd();
        int getPos();
        int setPos(int pos);

    private:
        FILE *m_fp;
        std::string m_name;
        void *m_mmaddr;
    };

    class FilePath
    {
    public:
        FilePath(const char *filePath);
        ~FilePath();
        std::string dirName();
        std::string baseName();
        static bool createLink(const char *filePath, const char *linkPath, bool hard = false);

    private:
        std::string m_filePath;
        char *m_dir;
        char *m_base;
    };

    /**
 *  \file   FileUtil.h
 *  \class  Directory
 *  \brief  文件目录类.
 */
    class Directory
    {
    public:
        Directory();
        ~Directory();
        static bool isEmpty(const char *name);
        static bool exists(const char *name);
        static int getContentSize(const char *name);
        static bool createDir(const char *name, bool recursive = false);
        static bool deleteDir(const char *name, bool recursive = false);
        bool enter(const std::string &path);
        std::string current();
        std::vector<std::string> listAll();

    private:
        std::string m_currentPath;
    };

} // namespace libemb

#endif
