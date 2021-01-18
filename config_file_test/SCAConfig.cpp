/**
 * SCAConfig.cpp:
 * tom.li@cloudminds.com 2020/12/12
 */

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

//#######@:#include "LogUtils.h"

//#######@:#include "sca-common.h"
//#######@:#include "sca-utils.h"
#include "SCAConfig.h"

using namespace std;

//##########################@:
#define LOG_D printf
#define LOG_I printf
#define LOG_E printf
#define AUTO_DOOR_THRESHOLD_CURRENT 6.5 // unit by Amper
const float GL_HEAD_VERT_CURRENT_THRESHOLD = 4.0;
#define SCA_HORZ_ZERO_OFFSET -0.7 // offset for old version's hardware asymmetric pos limit(185/175 degree)
#define SCA_CONFIG_FILE "/ftm/sca_config.txt"
#define MAX_SCA_CONFIG_LINE 512

#define KEY_AUTO_DOOR_CURRENT_THRESHOLD "auto_door_current_threshold"
#define KEY_GL_HEAD_VERT_CURRENT_THRESHOLD "gl_head_vert_current_threshold"
#define KEY_GL_HEAD_HORZ_ZERO_OFFSET "gl_head_horz_zero_offset"

namespace libemb
{
string &trim(string &s)
{
    if (s.empty())
    {
        return s;
    }
    else
    {
        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
        return s;
    }
}
} // namespace libemb

int str_sep(const std::string &src_str, const char sep, std::vector<std::string> &res)
{
    std::stringstream ss(src_str);
    std::string item;
    while (getline(ss, item, sep))
    {
        //std::cout << "item=" << item << std::endl;
        libemb::trim(item);
        res.push_back(item);
    }
    return (int)res.size();
}

//##########################@:

// Singleton Decl.
CSCAConfig *CSCAConfig::inst_ = NULL;
CSCAConfig *CSCAConfig::inst()
{
    if (CSCAConfig::inst_ == NULL)
    {
        CSCAConfig::inst_ = new CSCAConfig();
    }
    return CSCAConfig::inst_;
}

CSCAConfig::CSCAConfig()
{
    gl_auto_door_current_threshold_ = AUTO_DOOR_THRESHOLD_CURRENT;
    gl_head_vert_current_threshold_ = GL_HEAD_VERT_CURRENT_THRESHOLD;
    gl_head_horz_offset_ = SCA_HORZ_ZERO_OFFSET;
}

CSCAConfig::~CSCAConfig() {}

void CSCAConfig::init()
{
    const char *fn = SCA_CONFIG_FILE;
    LOG_I("open config file \"%s\" for reading\n", fn);

    FILE *fp = fopen(fn, "r");
    if (fp == NULL)
    {
        LOG_E("open config file \"%s\" failed\n", fn);
        return;
    }

    while (!feof(fp))
    {
        char line_str[MAX_SCA_CONFIG_LINE] = "";
        fgets(line_str, sizeof(line_str) - 1, fp);
        parse_config_line(line_str);
    }

    fclose(fp);

    show_config();
}

/**
 * config line, format as:
 * key = value
 * 
 */
void CSCAConfig::parse_config_line(const char *line_str)
{
    if (line_str == NULL)
    {
        //LOG_D("line string null\n");
        return;
    }

    string cfg_str = "";

    const char *ch = line_str;
    while ((*ch) != '\0' && (*ch) != '\n')
    {
        cfg_str += (*ch);
        ch++;
    }

    libemb::trim(cfg_str);
    if (cfg_str.empty())
    {
        //LOG_D("empty string\n");
        return;
    }

    if (cfg_str[0] == '#')
    {
        //LOG_D("comment string\n");
        return;
    }

    std::vector<std::string> cfg;
    if (str_sep(cfg_str, '=', cfg) != 2)
    {
        LOG_D("invalid config string \"%s\"\n", cfg_str.c_str());
        return;
    }

    parse_config_param(cfg[0], cfg[1]);
}

#define PARSE_CONFIG_ITEM(_var, _name, _default) \
    if (key == _name)                            \
    {                                            \
        try                                      \
        {                                        \
            _var = stof(value);                  \
        }                                        \
        catch (...)                              \
        {                                        \
            _var = _default;                     \
        }                                        \
    }

void CSCAConfig::parse_config_param(const string &key, const string &value)
{
    //LOG_D("parse_config_param: key=\"%s\", value=\"%s\"\n", key.c_str(), value.c_str());
    PARSE_CONFIG_ITEM(gl_auto_door_current_threshold_, KEY_AUTO_DOOR_CURRENT_THRESHOLD, AUTO_DOOR_THRESHOLD_CURRENT);
    PARSE_CONFIG_ITEM(gl_head_vert_current_threshold_, KEY_GL_HEAD_VERT_CURRENT_THRESHOLD, GL_HEAD_VERT_CURRENT_THRESHOLD);
    PARSE_CONFIG_ITEM(gl_head_horz_offset_, KEY_GL_HEAD_HORZ_ZERO_OFFSET, SCA_HORZ_ZERO_OFFSET);
}

void CSCAConfig::show_config()
{
    LOG_I("gl_auto_door_current_threshold_ = %.2f\n", gl_auto_door_current_threshold_);
    LOG_I("gl_head_vert_current_threshold_ = %.2f\n", gl_head_vert_current_threshold_);
    LOG_I("gl_head_horz_offset_ = %.2f\n", gl_head_horz_offset_);
}
