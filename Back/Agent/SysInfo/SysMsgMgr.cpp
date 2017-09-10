#include "SysMsgMgr.h"

SysMsgMgr::SysMsgMgr()
{
}

SysMsgMgr::~SysMsgMgr()
{
}

int SysMsgMgr::GetSysMsg(SysMsgConfig& sysMsgConfig)
{
        int ret = 0;
        string configPath = "./mail.conf";
        ConfigReader<SysMsgConfig> cr(configPath);

        ret = cr.Read(sysMsgConfig);
        if (ret < 0) {
                LOG_ERR("ConfigReader Read Error! path=%s\n", configPath.c_str());
                ret = -1;
        }
        return ret;
}

int SysMsgMgr::SetSysMsg(const SysMsgConfig& sysMsgConfig)
{
        int ret = 0;
        string configPath = "./mail.conf";
        ConfigReader<SysMsgConfig> cr(configPath);

        ret = cr.Write(sysMsgConfig);
        if (ret < 0) {
                LOG_ERR("ConfigReader Read Error! path=%s\n", configPath.c_str());
                ret = -1;
        }
        return ret;
}
