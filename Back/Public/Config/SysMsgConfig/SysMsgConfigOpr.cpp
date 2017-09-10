#include "SysMsgConfigOpr.h"

#include <stdio.h>
#include "Utils/Log/Log.h"
#include "Utils/Config/IniConfig.h"
#include "Utils/Common/CommonFunc.h"

SysMsgConfigOpr::SysMsgConfigOpr(const string& configPath):
        ConfigOpr<SysMsgConfig>(configPath)
{
}

SysMsgConfigOpr::~SysMsgConfigOpr()
{
}

int SysMsgConfigOpr::Read(SysMsgConfig& sysMsgConfig)
{
        int ret = 0;
        IniConfig ini(m_ConfigPath);
        ret = ini.Open();
        if (ret < 0) {
                LOG_ERR("Ini Open Error! path=%s\n", m_ConfigPath.c_str());
                return ret;
        }
        /* 全局域 */
        string global = "*";
        ini.GetVal(global, "server", sysMsgConfig.server);
        ini.GetVal(global, "user", sysMsgConfig.user);
        ini.GetVal(global, "password", sysMsgConfig.password);

        string autoType;
        ini.GetVal(global, "autoType", autoType);
        sysMsgConfig.autoType = atoi(autoType.c_str());

        string port;
        ini.GetVal(global, "port", port);
        sysMsgConfig.port = atoi(port.c_str());

        /* 获取接受邮箱结合 */
        int cnt = 0;
        char receiver[11];
        while (true) {
                string val, level;
                sprintf(receiver, "receiver_%d", cnt++);
                ret = ini.GetVal(global, receiver, val);
                /* 找不到后退出循环 */
                if (ret < 0) {
                        break;
                }
                size_t idx = val.find(";");
                /* 字符串中没有; 配置文件的格式不正确 */
                if (idx == string::npos) {
                        LOG_ERR("Config Type Error! path=%s\n", m_ConfigPath.c_str());
                        return -1;
                }
                level = val.substr(idx+1);
                val   = val.substr(0, idx);
                sysMsgConfig.receiverSet.insert(make_pair(val, atoi(level.c_str())));
        }
        return 0;
}

int SysMsgConfigOpr::Write(const SysMsgConfig& sysMsgConfig)
{
        int ret = 0;
        IniConfig ini(m_ConfigPath);
        ret = ini.Open();
        if (ret < 0) {
                LOG_ERR("Ini Open Error! path=%s\n", m_ConfigPath.c_str());
                return ret;
        }
        /* 全局域 */
        string global = "*";

        ini.SetVal(global, "server", sysMsgConfig.server);
        ini.SetVal(global, "user", sysMsgConfig.user);
        ini.SetVal(global, "password", sysMsgConfig.password);
        ini.SetVal(global, "autoType", IntToString(sysMsgConfig.autoType));
        ini.SetVal(global, "port", IntToString(sysMsgConfig.port));

        map<string,int>::const_iterator iter = sysMsgConfig.receiverSet.begin();
        int cnt = 0;
        char receiver[11];
        for (; iter != sysMsgConfig.receiverSet.end(); ++iter) {
                sprintf(receiver, "receiver_%d", cnt++);
                string val = iter->first + ";" + IntToString(iter->second);
                ini.SetVal(global, receiver, val);
        }
        ini.Close();

        return 0;
}
