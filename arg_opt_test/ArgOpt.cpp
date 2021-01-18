/**
 * Author : TomLi
 * Email  : 93873637@qq.com
 * Copyright 2020~2030, China
 */

#include "ArgOpt.h"
#include <iostream>
#include <sstream>

using namespace std;

namespace liblz
{

CArgOpt::CArgOpt()
{
}

CArgOpt::CArgOpt(const vector<string> &args)
{
    parse_options(args);
}

CArgOpt::CArgOpt(string args_str)
{
    vector<string> args;
    if (str_sep(args_str, args) > 0)
    {
        parse_options(args);
    }
}

CArgOpt::~CArgOpt()
{
}

/** 
 * cmd -t 10 -n 10 -b 0 -h 01 00
 */
void CArgOpt::parse_options(const vector<string> &args)
{
    opt_map_.clear();

    string key = KEY_OPT_HEAD;
    vector<string> vector;
    for (auto it = args.begin(); it != args.end(); it++)
    {
        if (is_option(*it))
        {
            if (!key.empty())
            {
                opt_map_[key] = vector;
                vector.clear();
            }
            key = *it;
        }
        else
        {
            if (!key.empty())
            {
                vector.push_back(*it);
            }
        }
    }

    // push last option
    opt_map_.insert(make_pair(key, vector));
}

bool CArgOpt::has_option(const string key)
{
    return opt_map_.find(key) != opt_map_.end();
}

void CArgOpt::show_options()
{
    for (auto it = opt_map_.begin(); it != opt_map_.end(); it++)
    {
        if (it->second.size() == 0)
        {
            cout << it->first.c_str() << "[" << it->second.size() << "]" << endl;
        }
        else
        {
            cout << it->first.c_str() << "[" << it->second.size() << "] = {";
            for (auto itv = it->second.begin(); itv != it->second.end(); itv++)
            {
                if (itv != it->second.begin())
                {
                    cout << ", ";
                }
                cout << *itv;
            }
            cout << "}" << endl;
        }
    }
}

int CArgOpt::get_int(const string key, int default_value)
{
    if (opt_map_.find(key) == opt_map_.end())
    {
        return default_value;
    }
    else
    {
        try
        {
            return stoi(opt_map_[key][0]);
        }
        catch (...)
        {
            return default_value;
        }
    }
}

float CArgOpt::get_float(const string key, float default_value)
{
    if (opt_map_.find(key) == opt_map_.end())
    {
        return default_value;
    }
    else
    {
        try
        {
            return stof(opt_map_[key][0]);
        }
        catch (...)
        {
            return default_value;
        }
    }
}

unsigned long CArgOpt::get_ulong(const string key, unsigned long default_value)
{
    if (opt_map_.find(key) == opt_map_.end())
    {
        return default_value;
    }
    else
    {
        try
        {
            return stoul(opt_map_[key][0]);
        }
        catch (...)
        {
            return default_value;
        }
    }
}

string CArgOpt::get_string(const string key, string default_value)
{
    if (opt_map_.find(key) == opt_map_.end())
    {
        return default_value;
    }
    else
    {
        return opt_map_[key][0];
    }
}

int CArgOpt::hex_char_to_int(char c)
{
    switch (c)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'A':
    case 'a':
        return 10;
    case 'B':
    case 'b':
        return 11;
    case 'C':
    case 'c':
        return 12;
    case 'D':
    case 'd':
        return 13;
    case 'E':
    case 'e':
        return 14;
    case 'F':
    case 'f':
        return 15;
    default:
        return -1;
    }
}

bool CArgOpt::hex_str_to_byte(const string &hex_str, unsigned char &hex)
{
    if (hex_str.size() != 2)
    {
        return false;
    }

    int high = hex_char_to_int(hex_str[0]);
    if (high < 0)
    {
        return false;
    }

    int low = hex_char_to_int(hex_str[1]);
    if (low < 0)
    {
        return false;
    }

    hex = (unsigned char)(high * 16 + low);
    return true;
}

bool CArgOpt::get_hexes(const string key, vector<unsigned char> &hexes)
{
    if (opt_map_.find(key) == opt_map_.end())
    {
        return false;
    }

    vector<string> &args = opt_map_[key];
    for (size_t i = 0; i < args.size(); i++)
    {
        unsigned char hex = 0;
        if (!hex_str_to_byte(args[i], hex))
        {
            return false;
        }
        hexes.push_back(hex);
    }

    return true;
}

bool CArgOpt::is_option(const string &arg)
{
    return !arg.empty() && arg[0] == KEY_OPTION;
}

int CArgOpt::str_sep(const std::string& src_str, std::vector<std::string> &res)
{
    std::string str_item;
    stringstream input(src_str);
    while (input >> str_item)
    {
        res.push_back(str_item);
    }
    return (int)res.size();
}

} // namespace liblz
