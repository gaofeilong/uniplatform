/* 
 * @ Date        : 2013-04-15
 * @ Auther      : wangbo
 * @ Description : 对Gateway配置文件进行操作(读,写)
 * @ Email       : wang.bo@scistor.com
 *
 * @ History     : 2013-04-15 创建
 *
 */

#include "GatewayConfigOpr.h"

#include <stdio.h>
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Utils/Config/IniConfig.h"
#include "Utils/Common/CommonFunc.h"

GatewayConfigOpr::GatewayConfigOpr(const string& configPath):
        ConfigOpr<struct _GatewayConfig>(configPath)
{
}

GatewayConfigOpr::~GatewayConfigOpr()
{
}

int GatewayConfigOpr::Read(struct _GatewayConfig &gatewayConfig)
{
        IniConfig ini(m_ConfigPath);
        if (ini.Open() < 0) {
                LOG_ERR("Open Error! configPath=%s\n", m_ConfigPath.c_str());
                return -1;
        }
        ini.GetVal("System", "version", gatewayConfig.version);
        ini.GetVal("System", "networkId", gatewayConfig.networkId);
        ini.GetVal("System", "systemType", gatewayConfig.systemType);
        ini.GetVal("System", "updateInterval", gatewayConfig.updateInterval);
        return 0;
}

int GatewayConfigOpr::Write(const struct _GatewayConfig &gatewayConfig)
{
        LOG_ERR("not support this interface");
        return -1;
}
