#ifndef _NETWORK_CONFIG_OPR_H_
#define _NETWORK_CONFIG_OPR_H_

#include "Public/Config/ConfigOpr.h"

/* 普通网卡，从属网卡 */
enum _NCType { REGULAR, BOND, SUBBOND };
/* config for network */
struct _NetworkConfig {
        enum _NCType type;
        string name;
        string mac;
        string ip;
        string mask;
        string gateway;
        string bootProto;
        string onBoot;
        string bondingOpts;
        string master;
        string slave;
        int    mtu;

        string linkState;
        string vendor;
        string speed;
};

class NetworkConfigOpr : public ConfigOpr<struct _NetworkConfig> {
public:
        NetworkConfigOpr(const string& configPath);
        ~NetworkConfigOpr();

public:
        int Read(struct _NetworkConfig &networkConfig);
        int Write(const struct _NetworkConfig &networkConfig);
};

#endif //_NETWORK_CONFIG_OPR_H_
