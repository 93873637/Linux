/**
 * Author : TomLi
 * Email  : 93873637@qq.com
 * Copyright 2020~2030, China
 */

/**
 * cmds string format as:
 *   head0 head1 .. headN -key1 value1 -key2 value2 ... -keyN valueN
 * such as:
 *   head1 head2 head3 he -t 10 -a 99 -s aaa -b 5.0 -hex 00 99 08 35 -j 8
 * will be parsed as:
 *   ------------------------------------------
 *   -a[1] = {99}
 *   -b[1] = {5.0}
 *   -hex[4] = {00, 99, 08, 35}
 *   -j[1] = {8}
 *   -s[1] = {aaa}
 *   -t[1] = {10}
 *   head[4] = {head1, head2, head3, he}
 *   ------------------------------------------- 
 *  which can be printed by opt.show_options()
 */

#ifndef __ARG_OPT_H__
#define __ARG_OPT_H__

#include <map>
#include <string>
#include <vector>

#define KEY_OPTION '-'
#define KEY_OPT_HEAD "head"

using namespace std;

namespace liblz
{

// key/value: string/vector<string>
typedef map<string, vector<string>> ArgOptMap;

class CArgOpt
{
public:
    CArgOpt();
    CArgOpt(string args_str);
    CArgOpt(const vector<string> &args);
    ~CArgOpt();

public:
    void parse_options(const vector<string> &args);
    bool has_option(const string key);
    void show_options();

public:
    int get_int(const string key, int default_value);
    float get_float(const string key, float default_value);
    unsigned long get_ulong(const string key, unsigned long default_value);
    string get_string(const string key, string default_value);
    bool get_hexes(const string key, vector<unsigned char> &hexes);

public:
    static bool is_option(const string &arg);
    static int hex_char_to_int(char c);
    static bool hex_str_to_byte(const string &hex_str, unsigned char &hex);
    int str_sep(const std::string& src_str, std::vector<std::string> &res);

private:
    ArgOptMap opt_map_;
};

} // namespace liblz

#endif //__ARG_OPT_H__
