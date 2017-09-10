/* 
 * @ Date        : 2012-09-25
 * @ Auther      : wangbo
 * @ Description : 对Network配置文件进行操作(读,写)
 * @ Email       : wang.bo@scidata.cn
 *
 * @ History     : 2012-09-04 创建
 *
 */
#include <stdio.h>

#include "NetworkConfigOpr.h"
#include "Utils/Log/Log.h"
#include "Utils/Config/IniConfig.h"
#include "Utils/Common/CommonFunc.h"

#define GLOBAL_FIELD "*"

NetworkConfigOpr::NetworkConfigOpr(const string& configPath):
        ConfigOpr<struct _NetworkConfig>(configPath)
{
}

NetworkConfigOpr::~NetworkConfigOpr()
{
}

int NetworkConfigOpr::Read(struct _NetworkConfig &networkConfig)
{
        int ret = 0;
        IniConfig ini(m_ConfigPath);
        string tmpMtu, tmpMask, tmpPrefix;

        ret = ini.Open();
        if (ret < 0) {
                LOG_ERR("Open Error! configPath=%s\n", m_ConfigPath.c_str());
                return -1;
        }

        ini.GetVal(GLOBAL_FIELD, "DEVICE", networkConfig.name);
        ini.GetVal(GLOBAL_FIELD, "HWADDR", networkConfig.mac);
        ini.GetVal(GLOBAL_FIELD, "IPADDR", networkConfig.ip);
        ini.GetVal(GLOBAL_FIELD, "ONBOOT", networkConfig.onBoot);
        ini.GetVal(GLOBAL_FIELD, "GATEWAY", networkConfig.gateway);
        ini.GetVal(GLOBAL_FIELD, "BOOTPROTO", networkConfig.bootProto);
        ini.GetVal(GLOBAL_FIELD, "BONDING_OPTS", networkConfig.bondingOpts);
        ini.GetVal(GLOBAL_FIELD, "MASTER", networkConfig.master);
        ini.GetVal(GLOBAL_FIELD, "SLAVE", networkConfig.slave);
        ini.GetVal(GLOBAL_FIELD, "PREFIX", tmpPrefix);
        ini.GetVal(GLOBAL_FIELD, "NETMASK", tmpMask);
        ini.GetVal(GLOBAL_FIELD, "MTU", tmpMtu);
        // networkConfig.type is not set

        networkConfig.mtu = tmpMtu.empty()? 1500: atoi(tmpMtu.c_str());
        if (tmpMask.empty()) {
                networkConfig.mask = tmpPrefix.empty()? "": 
                        MasklenToMask(atoi(tmpPrefix.c_str()));
        } else {
                networkConfig.mask = tmpMask;
        }

        return 0;
}

int NetworkConfigOpr::Write(const struct _NetworkConfig &networkConfig)
{
        FILE *stream = fopen(m_ConfigPath.c_str(), "w");
        if (stream == NULL) {
                LOG_ERR("open net config file %s error", m_ConfigPath.c_str());
        }

        fprintf(stream, "DEVICE=%s\n", networkConfig.name.c_str());
        fprintf(stream, "BOOTPROTO=%s\n", networkConfig.bootProto.c_str());
        fprintf(stream, "ONBOOT=yes\n");
        fprintf(stream, "MTU=%d\n", networkConfig.mtu);

        if (networkConfig.bootProto != "dhcp") {
                if (!networkConfig.ip.empty()) {
                        fprintf(stream, "IPADDR=%s\n", networkConfig.ip.c_str());
                }
                if (!networkConfig.mask.empty()) {
                        fprintf(stream, "NETMASK=%s\n", networkConfig.mask.c_str());
                }
                if (!networkConfig.gateway.empty()) {
                        fprintf(stream, "GATEWAY=%s\n", networkConfig.gateway.c_str());
                }
        }

        switch (networkConfig.type) {
        case REGULAR:
                fprintf(stream, "HWADDR=%s\n", networkConfig.mac.c_str());
                break;
        case BOND:
                fprintf(stream, "BONDING_OPTS=%s\n", networkConfig.bondingOpts.c_str());
                break;
        case SUBBOND:
                fprintf(stream, "HWADDR=%s\n", networkConfig.mac.c_str());
                fprintf(stream, "MASTER=%s\n", networkConfig.master.c_str());
                fprintf(stream, "SLAVE=yes\n");
                break;
        default:
                break;
        }

        return fclose(stream);
}
