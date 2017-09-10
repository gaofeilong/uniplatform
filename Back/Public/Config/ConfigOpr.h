#ifndef _CONFIG_OPR_H_
#define _CONFIG_OPR_H_

#include <string>
using std::string;

template <typename T>
class ConfigOpr {
public:
        ConfigOpr(){};
        ConfigOpr(const string& configPath):m_ConfigPath(configPath){};
        virtual ~ConfigOpr(){};
        
public:
        virtual int  Read(T& configType) = 0;
        virtual int  Write(const T& configType) = 0;

protected:
        string m_ConfigPath;
};

#endif //_CONFIG_OPR_H_
