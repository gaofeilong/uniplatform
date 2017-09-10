#ifndef _GATEWAY_CONFIG_OPR_H_
#define _GATEWAY_CONFIG_OPR_H_

#include <string>
#include <vector>
#include "Public/Config/ConfigOpr.h"

using std::string;
using std::vector;

struct _GatewayConfig {
        _GatewayConfig()
        {
        }
        string version;
        string networkId;
        string systemType;
        string updateInterval;
};


class GatewayConfigOpr : public ConfigOpr<struct _GatewayConfig> {
public:
        GatewayConfigOpr(const string& configPath);
        ~GatewayConfigOpr();

public:
        int  Read(struct _GatewayConfig& gatewayConfig);
        int  Write(const struct _GatewayConfig& gatewayConfig);
};

#endif //_GATEWAY_CONFIG_OPR_H_
