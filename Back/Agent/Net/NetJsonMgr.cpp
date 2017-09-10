/* 
 * @ Date        : 2013-04-01
 * @ Auther      : wangbo
 * @ Description : 处理网络模块协议
 * @ Email       : wang.bo@scistor.com
 *
 * @ History     : 2013-04-01 创建
 *               : 2013-04-07 实习协议分发
 *
 */

#include <stdio.h>
#include <string.h>

#include "NetJsonMgr.h"
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Include/String.h"
#include "Include/Protocol.h"
#include "Utils/Common/CommonFunc.h"
#include "Server/Handler/BadRequestHandler.h"

NetJsonMgr::NetJsonMgr()
{
}

NetJsonMgr::~NetJsonMgr()
{
}

Packet *NetJsonMgr::Handle(const Packet *p)
{
        switch(p->Cmd){
        case CMD_NET_BASE_LOOKUP:
                return LookupNetBaseInfo(p);
        case CMD_NET_BASE_SET:
                return SetNetBaseInfo(p);
        case CMD_NET_CARD_LOOKUP:
                return LookupNetInfo(p);        //查看网络信息
        case CMD_NET_CARD_SET:
                return SetNetConf(p);	        //配置网络
        case CMD_NET_BOND_CREATE:
                return BondNetCard(p);	        //链路聚合
        case CMD_NET_BOND_REMOVE:
                return UbondNetCard(p);	        //解除聚合
        case CMD_NET_BOND_SET:
                return ModifyBond(p);
        case CMD_NET_FC_LOOKUP:
                return LookupFcInfo(p);	        //查看FC设置信息
        case CMD_NET_FC_SET:
                return ModifyFcConf(p);	        //修改FC配置
        case CMD_NET_ISCSI_LOOKUP:
                return LookupIscsiInfo(p);	//查看ISCSI设置信息
        case CMD_NET_ISCSI_SET:
                return ModifyIscsiConf(p);	//修改ISCSI配置
        default:
                return RequestError("unknown request");
        }
        return 0;
}

Packet *NetJsonMgr::LookupNetBaseInfo(const Packet *p)
{
        LOG_DBG("LookupNetBaseInfo()\n");
        int ret = 0;

        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        string host;
        vector<string> dns;
        ret = m_NetworkMgr.GetHostName(host);
        if (ret != 0) {
                return RequestError("get dns error");
        }
        ret = m_NetworkMgr.GetDns(dns);
        if (ret != 0) {
                return RequestError("get nost name error");
        }

        respond["hostName"] = host;
        if (dns.size() >= 1) {
                respond["dns0"] = dns[0];
                if (dns.size() == 2) {
                        respond["dns1"] = dns[1];
                }
        }

        return RequestOk(respond);
}

Packet *NetJsonMgr::SetNetBaseInfo(const Packet *p)
{
        LOG_DBG("SetNetBaseInfo()\n");
        int ret = 0;

        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        vector<string> dns;
        dns.push_back(request["dns0"].asString());
        dns.push_back(request["dns1"].asString());
        ret = m_NetworkMgr.SetDns(dns);
        if (ret < 0) {
                return RequestError("set dns error");
        }
        ret = m_NetworkMgr.SetHostName(request["hostName"].asString());
        if (ret < 0) {
                return RequestError("set host name error");
        }

        return RequestOk(respond);
}

Packet *NetJsonMgr::LookupNetInfo(const Packet *p)
{
        LOG_DBG("LookupNetInfo()\n");
        int ret = 0;

        map<string, struct _NetworkConfig> NCSet;
        NetworkMgr nm;
        ret = nm.GetNetCardSet(NCSet);
        if (ret < 0) {
                LOG_ERR("GetNetCardSet Error!\n");
                return RequestError("get netcard set error");
        }

        Json::Value respond;
        map<string, struct _NetworkConfig>::iterator iter = NCSet.begin();
        for (; iter != NCSet.end(); ++iter) {
                Json::Value tmp;
                string name = iter->first;
                struct _NetworkConfig conf = iter->second;

                if (name.find("bond") != string::npos) {
                        // 获取绑定的网卡
                        vector<string> subCards;
                        ret = nm.GetBuildBondNC(name, subCards);
                        if (ret < 0) {
                                LOG_ERR("GetBuildBondNC Error!\n");
                                return RequestError("get build card error");
                        }
                        for (size_t i=0; i<subCards.size(); ++i) {
                                tmp["eths"].append(subCards[i]);
                        }

                        size_t idx = conf.bondingOpts.find("mode=") + 
                                                        strlen("mode=");
                        tmp["mode"] = conf.bondingOpts[idx] - '0';
                } else {
                        tmp["vendor"] = conf.vendor;
                        tmp["speed"]  = conf.speed;
                        tmp["mac"]    = conf.mac;
                }
                tmp["name"]      = name;
                tmp["ip"]        = conf.ip;
                tmp["mask"]      = conf.mask;
                tmp["gateway"]   = conf.gateway;
                tmp["bootProto"] = conf.bootProto;
                tmp["mtu"]       = conf.mtu;
                tmp["state"]     = conf.linkState;

                if (name.find("bond") != string::npos) {
                        respond["bonds"].append(tmp);
                } else {
                        respond["eths"].append(tmp);
                }
        }
        return RequestOk(respond);
}

Packet *NetJsonMgr::SetNetConf(const Packet *p)
{
        LOG_DBG("SetNetConf()\n");
        int ret = 0;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }
        
        struct _NetworkConfig nc;
        nc.type      = REGULAR;
        nc.name      = request["name"].asString();
        nc.bootProto = request["bootProto"].asString();
        nc.mtu       = request["mtu"].asInt();
        nc.mac       = request["mac"].asString();
        nc.onBoot    = "yes";
        if (nc.bootProto == "none") {
                nc.ip      = request["ip"].asString();
                nc.mask    = request["mask"].asString();
                nc.gateway = request["gateway"].asString();
        }

        string errInfo;
        NetworkMgr nm;
        ret = nm.Ifdown(nc.name, errInfo);
        if (ret < 0) {
                LOG_ERR("Ifdown error! name=%s\n", nc.name.c_str());
                return RequestError(errInfo);
        }
        // 备份源配置文件
        ret = nm.CpEthConf(nc.name, TMP_ETH_CONF);
        if (ret < 0) {
                LOG_ERR("CpEthConf Error! nc=%s\n", nc.name.c_str());
                return RequestError("backup network config error");
        }

        ret = nm.SetNetworkConfig(nc.name, nc);
        if (ret < 0) {
                LOG_ERR("SetNetworkConfig Error!\n");
                return RequestError("set network config error");
        }

        ret = nm.Ifup(nc.name, errInfo);
        if (ret < 0) {
                LOG_ERR("Ifup error! name=%s\n", nc.name.c_str());
                nm.CpEthConf(TMP_ETH_CONF, nc.name);
                nm.Ifup(nc.name, errInfo);
                return RequestError(errInfo);
        }

        return RequestOk(respond);
}

Packet *NetJsonMgr::BondNetCard(const Packet *p)
{
        LOG_DBG("BondNetCard()\n");
        int ret = 0;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        //确定是第几个绑定网卡
        int cnt = 0;
        string name;
        string cPath = "/etc/sysconfig/network-scripts/ifcfg-";
        while (1) {
                name = "bond" + IntToString(cnt);
                if (!HasPath(cPath+name)) {
                        break;
                }
                ++cnt;
        }

        vector<string> ethVec;
        struct _NetworkConfig nc;
        nc.onBoot      = "yes";
        nc.type        = BOND;
        nc.name        = name;
        nc.mtu         = request["mtu"].asInt();
        nc.bootProto   = request["bootProto"].asString();
        nc.bondingOpts = "mode=" + IntToString(request["mode"].asInt()) 
                                                        + " miimon=100";
        if (nc.bootProto == "none") {
                nc.ip      = request["ip"].asString();
                nc.mask    = request["mask"].asString();
                nc.gateway = request["gateway"].asString();
        }
        for (size_t i=0; i<request["eths"].size(); ++i) {
                ethVec.push_back(request["eths"][i].asString());
        }

        // ifdown所有从属网卡
        string errInfo;
        NetworkMgr nm;
        for (size_t i=0; i<ethVec.size(); ++i) {
                ret = nm.Ifdown(ethVec[i], errInfo);
                if (ret < 0) {
                        LOG_ERR("ifdown [%s] error", ethVec[i].c_str());
                        return RequestError(errInfo);
                }
        }

        // 绑定
        ret = nm.AddBond(ethVec, nc);
        if (ret < 0) {
                LOG_ERR("add bond error!\n");
                return RequestError("create bond error");
        }

        // ifup绑定网卡
        ret = nm.Ifup(nc.name, errInfo);
        if (ret < 0) {
                LOG_WRN("ifup [%s] error!", nc.name.c_str());
                // return RequestError(errInfo);
        }

        return RequestOk(respond);
}

Packet *NetJsonMgr::UbondNetCard(const Packet *p)
{
        LOG_DBG("UbondNetCard()\n");

        int ret = 0;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        string name = request["name"].asString();

        // 获取从属网卡
        NetworkMgr nm;
        vector<string> ncSet;
        ret = nm.GetBuildBondNC(name, ncSet);
        if (ret < 0) {
                LOG_ERR("GetBuildBondNC error! name=%s\n", name.c_str());
                return RequestError("get build bondnc error");
        }

        string errInfo;
        // ifdown绑定网卡
        ret = nm.Ifdown(name, errInfo);
        if (ret < 0) {
                LOG_ERR("Ifdown error! name=%s\n", name.c_str());
                return RequestError(errInfo);
        }

        // 解绑
        ret = nm.DelBond(name, ncSet);
        if (ret < 0) {
                LOG_ERR("delete bond error\n");
                return RequestError("delete bond error");
        }

        // ifup从属网卡
        for (size_t i=0; i<ncSet.size(); ++i) {
                ret = nm.Ifup(ncSet[i], errInfo);
                if (ret < 0) {
                        LOG_ERR("Ifup error! name=%s\n", ncSet[i].c_str());
                        return RequestError(errInfo);
                }
        }
        return RequestOk(respond);
}

Packet *NetJsonMgr::ModifyBond(const Packet *p)
{
        LOG_DBG("ModifyBond()\n");

        int ret = 0;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        struct _NetworkConfig nc;
        nc.type        = BOND;
        nc.onBoot      = "yes";
        nc.mtu         = request["mtu"].asInt();
        nc.name        = request["name"].asString();
        nc.bootProto   = request["bootProto"].asString();
        nc.bondingOpts = "mode=" + IntToString(request["mode"].asInt()) + 
                                                             " miimon=100";
        if (nc.bootProto == "none") {
                nc.ip      = request["ip"].asString();
                nc.mask    = request["mask"].asString();
                nc.gateway = request["gateway"].asString();
        }
        
        string errInfo;
        NetworkMgr nm;
        ret = nm.Ifdown(nc.name, errInfo);
        if (ret < 0) {
                LOG_ERR("Ifdown error! name=%s\n", nc.name.c_str());
                return RequestError(errInfo);
        }

        // 备份源配置文件
        ret = nm.CpEthConf(nc.name, TMP_ETH_CONF);
        if (ret < 0) {
                LOG_ERR("CpEthConf Error! nc=%s\n", nc.name.c_str());
                return RequestError("backup network config error");
        }

        ret = nm.ModifyBond(nc);
        if (ret < 0) {
                LOG_ERR("ModifyBond Error!\n");
                return RequestError("modify bond net card error");
        }

        ret = nm.Ifup(nc.name, errInfo);
        if (ret < 0) {
                LOG_ERR("Ifup error! name=%s\n", nc.name.c_str());
                nm.CpEthConf(TMP_ETH_CONF, nc.name);
                nm.Ifup(nc.name, errInfo);
                return RequestError(errInfo);
        }
        return RequestOk(respond);
}

Packet *NetJsonMgr::LookupBondInfo(const Packet *p)
{
        LOG_DBG("LookupBondInfo()\n");
        return RequestError("unknown request");
}

Packet *NetJsonMgr::LookupFcInfo(const Packet *p)
{
        LOG_DBG("LookupFcInfo()\n");
        return RequestError("unknown request");
}

Packet *NetJsonMgr::ModifyFcConf(const Packet *p)
{
        LOG_DBG("ModifyFcConf()\n");
        return RequestError("unknown request");
}


Packet *NetJsonMgr::LookupIscsiInfo(const Packet *p)
{
        LOG_DBG("LookupIscsiInfo()\n");
        return RequestError("unknown request");
}

Packet *NetJsonMgr::ModifyIscsiConf(const Packet *p)
{
        LOG_DBG("ModifyIscsiConf()\n");
        return RequestError("unknown request");
}
