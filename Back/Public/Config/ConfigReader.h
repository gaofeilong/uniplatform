/* 
 * @ Date        : 2012-09-04
 * @ Auther      : wangbo
 * @ Email       : wang.bo@scidata.cn
 * @ Description : 对配置文件进行操作(读,写),该类为基类
 *
 * @ History     : 2012-09-04 创建
 *               : 2012-09-05 修改模板指针强制转换
 *
 */

#ifndef _CONFIG_READER_H_
#define _CONFIG_READER_H_

#include <string>
#include <typeinfo>

#include "ConfigStruct.h"
#include "Utils/Log/Log.h"

using std::string;

template <class T>
class ConfigReader {
public:
        explicit ConfigReader(const string& configPath);
        ~ConfigReader();

public:
        int  Read(T& configType);
        int  Write(const T& configType);

private:
        void MakeConfigOpr();

private:
        ConfigOpr<T>* m_ConfigOpr;
        string        m_ConfigPath;
};

template <typename T>
ConfigReader<T>::ConfigReader(const string& configPath)
{
        m_ConfigPath = configPath;
        m_ConfigOpr  = NULL;
        MakeConfigOpr();
}

template <typename T>
ConfigReader<T>::~ConfigReader()
{
        if (m_ConfigOpr != NULL) {
                delete m_ConfigOpr;
        }
}

template <typename T>
void ConfigReader<T>::MakeConfigOpr()
{        
        if (typeid(T) == typeid(struct _DdfsConfig)) {
                m_ConfigOpr = (ConfigOpr<T>*)new DdfsConfigOpr(m_ConfigPath);
        } else if (typeid(T) == typeid(struct _NetworkConfig)) {
                m_ConfigOpr = (ConfigOpr<T>*)new NetworkConfigOpr(m_ConfigPath);
        } else if (typeid(T) == typeid(struct _SysMsgConfig)) {
                m_ConfigOpr = (ConfigOpr<T>*)new SysMsgConfigOpr(m_ConfigPath);
        } else if (typeid(T) == typeid(struct _MysqlConfig)) {
                m_ConfigOpr = (ConfigOpr<T>*)new MysqlConfigOpr(m_ConfigPath);
        } else if (typeid(T) == typeid(struct _ArchiveConfig)) {
                m_ConfigOpr = (ConfigOpr<T>*)new ArchiveConfigOpr(m_ConfigPath);
        } else if (typeid(T) == typeid(struct _GatewayConfig)) {
                m_ConfigOpr = (ConfigOpr<T>*)new GatewayConfigOpr(m_ConfigPath);
        } else {
                LOG_INF("don't have this config\n");
        }
}

template <typename T>
int ConfigReader<T>::Read(T& configType)
{
        assert(m_ConfigOpr != NULL);
        return m_ConfigOpr->Read(configType);
}

template <typename T>
int ConfigReader<T>::Write(const T& configType)
{
        assert(m_ConfigOpr != NULL);
        return m_ConfigOpr->Write(configType);
}

#endif //_CONFIG_READER_H_
