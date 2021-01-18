#ifndef __SCA_CONFIG_H__
#define __SCA_CONFIG_H__

#include <string>

using namespace std;

class CSCAConfig
{
    // Singleton Decl.
public:
    static CSCAConfig *inst();
private:
    static CSCAConfig *inst_; // the one and only object instance
protected:
    CSCAConfig();
    virtual ~CSCAConfig();

public:
    void init();
    void show_config();
    
protected:
    void parse_config_line(const char *line_str);
    void parse_config_param(const string& key, const string& value);
    
public:
    float gl_auto_door_current_threshold_;
    float gl_head_vert_current_threshold_;
    float gl_head_horz_offset_;
};

#define SCAConfig (*CSCAConfig::inst())

#endif // __SCA_CONFIG_H__
