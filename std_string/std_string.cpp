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

bool begin_with(std::string &src, const char *with_str)
{
    return (src.find(with_str) == 0);
}

bool end_with(std::string &src, const char *with_str)
{
    return (src.rfind(with_str) == (src.length() - strlen(with_str)));
}

int split(const char *s, const char *delim, std::vector<std::string> &result)
{
    if (s && strlen(s))
    {
        int len = strlen(s);
        char *src = new char[len + 1];
        strcpy(src, s);
        src[len] = '\0';
        char *ptr = strtok(src, delim);
        while (ptr != NULL)
        {
            std::string tks = ptr;
            result.emplace_back(tks);
            ptr = strtok(NULL, delim);
        }
        delete[] src;
    }
    return result.size();
}

void get_path_name(const std::string path_full, std::string &path_name)
{
    std::size_t last = path_full.find_last_of("/");
    if (last == path_full.length() - 1)
    {
        //process the case of end with '/'
        std::string path_full_trim = path_full.substr(0, last);
        std::size_t last_trim = path_full_trim.find_last_of("/");
        path_name = path_full_trim.substr(last_trim + 1);
    }
    else
    {
        path_name = path_full.substr(last + 1);
    }
}

#define DEFAULT_NIGHT_MODE_EXPLIST     "200/67,400/100,800/125,1600/167,2200/266,3200/266"
#define DEFAULT_NIGHT_MODE_EXPLIST2    ""

void setBurstNum(int& nBurstNum)
{
    nBurstNum = 111;
}

void StringReplace(string &strBase, string strSrc, string strDes)
{
    string::size_type pos = 0;
    string::size_type srcLen = strSrc.size();
    string::size_type desLen = strDes.size();
    pos=strBase.find(strSrc, pos); 
    while ((pos != string::npos))
    {
        strBase.replace(pos, srcLen, strDes);
        pos=strBase.find(strSrc, (pos+desLen));
    }
}

int main()
{
    std::string aa = "aaa";
    aa += "bb";
    aa += to_string(6);
    std::cout << aa << std::endl;

//    {
//        std::string base = "iso200exp33frm10-iso600exp100frm10";
//        cout << "before: " << base << endl;
//        StringReplace(base, "iso", "i");
//        StringReplace(base, "exp", "e");
//        StringReplace(base, "frm", "f");
//        cout << "after: " << base << endl;
//    }    
    {
        std::string base = "iso200exp33frm10-iso600exp100frm10.jpg";
        const char *end_str = ".nv21";
        cout << "\"" << base << "\".end_with(\"" << end_str << "\")=" << end_with(base, end_str) << endl;
    }
    {
        std::string base = "iso200exp33frm10-iso600exp100frm10.nv21";
        const char *end_str = ".nv21";
        cout << "\"" << base << "\".end_with(\"" << end_str << "\")=" << end_with(base, end_str) << endl;
    }
    {
        std::string base = "iso200exp33frm10-iso600exp100frm10.nv21";
        const char *with_str = ".nv21";
        cout << "\"" << base << "\".begin_with(\"" << with_str << "\")=" << begin_with(base, with_str) << endl;
    }
    {
        std::string base = "NI_iso200exp33frm10-iso600exp100frm10.nv21";
        const char *with_str = "NI_";
        cout << "\"" << base << "\".begin_with(\"" << with_str << "\")=" << begin_with(base, with_str) << endl;
    }

//    int nBurstNum = 99;
//    cout << "input nBurstNum=" << nBurstNum << endl;
//    setBurstNum(nBurstNum);
//    cout << "output nBurstNum=" << nBurstNum << endl;
    
//    {
//        const char *s = DEFAULT_NIGHT_MODE_EXPLIST2;
//        const char *delim = ",";
//        
//        std::vector<std::string> resVect;
//        split(s, delim, resVect);
//        
//        cout << "string=\"" << s << "\", delim=\"" << delim << "\", size=" << resVect.size() << endl;
//        for (int i=0; i<resVect.size(); i++)
//            cout << (i+1) << ". " << resVect[i] << endl;
//    }
    
//    {
//        std::string path_full = "/home/cloud/proj/Camera_SuperNight/res/images/1216/a2h/iso20000_exp250";
//        std::string path_name;
//        get_path_name(path_full, path_name);
//        cout << "path_full=" << path_full << ", path_name=" << path_name << endl;
//    }
//
//    {
//        std::string path_full = "/home/cloud/proj/Camera_SuperNight/res/images/1216/a2h/iso20000_exp250/";
//        std::string path_name;
//        get_path_name(path_full, path_name);
//        cout << "path_full=" << path_full << ", path_name=" << path_name << endl;
//    }
//
//    


    return 0;
}
