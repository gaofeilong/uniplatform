/* 
 * @ Date        : 2012-09-25
 * @ Auther      : wangbo
 * @ Description : 对网络信息的查看，主要是对网卡的配置
 * @ Email       : wang.bo@scidata.cn
 *
 * @ History     : 2012-09-25 创建
 *                 2013-04-11 修改Bond功能，提供更方便的接口
 *
 */

#include "NetworkMgr.h"

#include "Include/String.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Common/CommonFunc.h"

#include <algorithm>

NetworkMgr::NetworkMgr()
{
}

NetworkMgr::~NetworkMgr()
{
}

int NetworkMgr::GetDns(vector<string>& dnsList)
{
        int ret = 0;
        ret = Execute(CL_GET_DNS, dnsList);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s: %s\n", 
                        CL_GET_DNS.c_str(), dnsList[0].c_str());
                ret = -1;
        }
        return 0;
}

int NetworkMgr::SetDns(const vector<string>& dnsList)
{
        int ret = 0;
        string cmd;
        vector<string> tmpVec;
        if (!dnsList[0].empty()) {
                cmd = dnsList[1].empty()? CL_SET_DNS1(dnsList[0]): 
                                CL_SET_DNS2(dnsList[0], dnsList[1]);
        } else {
                cmd = CL_CLEAR_DNS;
        }
        ret = Execute(cmd, tmpVec);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s: %s\n", cmd.c_str(), tmpVec[0].c_str());
                return -2;
        }
        return 0;
}

int NetworkMgr::GetHostName(string &host)
{
        vector<string> tmpVec;
        int ret = Execute(CL_GET_HOST, tmpVec);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s: %s\n", 
                        CL_GET_HOST.c_str(), tmpVec[0].c_str());
                return -1;
        }
        host = tmpVec[0];
        return 0;
}

int NetworkMgr::SetHostName(const string &host)
{
        vector<string> tmpVec;
        int ret = Execute(CL_SET_HOST(host), tmpVec);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s: %s\n", 
                        CL_SET_HOST(host).c_str(), tmpVec[0].c_str());
                return -1;
        }
        return 0;
}

int NetworkMgr::GetNetCardSet(map<string, struct _NetworkConfig>& NCSet)
{
        int ret = 0;
        vector<string> allEths, subs, allSubs;
        map<string, struct _NetworkConfig> networkInfo;

        /* 获取所有网卡的名字，如eth0 */
        ret = GetAllNetCardName(allEths);
        if (ret < 0) {
                LOG_ERR("GetAllNetCardName Error!\n");
                return ret;
        }

        /* 获取所有网卡的信息，并记录绑定网卡的子网卡信息 */
        vector<string>::iterator vsIter;
        for (vsIter = allEths.begin(); vsIter != allEths.end(); ++vsIter) {
                struct _NetworkConfig NCInfo;

                ret = GetNetworkConfig(*vsIter, NCInfo);
                if (ret < 0) {
                        LOG_ERR("get net card %s info error", vsIter->c_str());
                        continue;
                }
                if (vsIter->find("bond") != string::npos) {
                        // 记录无效的网卡，
                        subs.clear();
                        ret = GetBuildBondNC(*vsIter, subs);
                        if (ret < 0) {
                                LOG_ERR("get bond %s subnet info error\n", 
                                                               vsIter->c_str());
                                continue;
                        }
                        vector<string>::iterator vsIt1 = subs.begin();
                        for (; vsIt1 != subs.end(); ++vsIt1) {
                                allSubs.push_back(*vsIt1);
                        }
                }
                networkInfo.insert(make_pair(*vsIter, NCInfo));
        }

        map<string, struct _NetworkConfig>::iterator msnIt;
        for (msnIt = networkInfo.begin(); msnIt != networkInfo.end(); ++msnIt) {
                if (find(allSubs.begin(), allSubs.end(), msnIt->first) == allSubs.end()) {
                        NCSet.insert(*msnIt);
                }
        }

        return 0;
}

int NetworkMgr::GetNetworkConfig(const string& name, struct _NetworkConfig& NCInfo)
{
        int ret = 0;
        string ncPath = NETWORK_DIR + name;
        ConfigReader<struct _NetworkConfig> crNC(ncPath);
        ret = crNC.Read(NCInfo);
        if (ret < 0) {
                LOG_ERR("config read error! path=%s\n", ncPath.c_str());
        }

        NCInfo.name = name;
        //通过命令抓取 状态, 型号, 速度, 子网掩码
        vector<string> infoVec;
        string cmd = CL_GET_NET_INFO(name);
        ret = Execute(cmd, infoVec);
        if (ret < 0) {
                LOG_ERR("execute cmd error! cmd=%s\n", cmd.c_str());
                return ret;
        }
        if (infoVec.empty()) {
                LOG_ERR("can't get net info!\n");
                return -1;
        }

        NCInfo.ip      = infoVec[IPINDEX];
        NCInfo.mask    = infoVec[MASKINDEX];
        NCInfo.gateway = infoVec[GWINDEX];
        NCInfo.linkState = infoVec[LINKINDEX];
        if (name.find("bond") == string::npos) {
                NCInfo.speed     = infoVec[SPEEDINDEX];
                NCInfo.vendor    = infoVec[VENDORINDEX];
                NCInfo.mac       = infoVec[MACINDEX];
        }

        return ret;
}

int NetworkMgr::GetBuildBondNC(const string& bondName, vector<string>& bondNCNameSet)
{
        int ret = 0;
        string cmd = CL_BOND_INFO(bondName);
        ret = Execute(cmd, bondNCNameSet);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s\n", cmd.c_str());
        }
        return ret;
}

int NetworkMgr::SetNetworkConfig(const string& name, const struct _NetworkConfig& NCInfo)
{
        int ret = 0;
        string ncPath = NETWORK_DIR + name;
        ConfigReader<struct _NetworkConfig> crNC(ncPath);
        ret = crNC.Write(NCInfo);
        if (ret < 0) {
                LOG_ERR("config read error! path=%s\n", ncPath.c_str());
        }
        return ret;
}

int NetworkMgr::DelBondConfig(const string& name)
{
        int ret = 0;
        string ncPath = NETWORK_DIR + name;
        string cmd = CL_DEL_PATH(ncPath);
        vector<string> errVec;
        ret = Execute(cmd, errVec);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s\n", cmd.c_str());
        }
        return ret;
}

int NetworkMgr::AddBond(const vector<string>& NcNameSet, const struct _NetworkConfig& NCInfo)
{
        int ret = 0;
        /* 1. 创建bond[n]文件并修改文件中内容 */
        ret = SetNetworkConfig(NCInfo.name, NCInfo);
        if (ret < 0) {
                LOG_ERR("SetNetworkConfig Error! name=%s\n", NCInfo.name.c_str());
                return -1;
        }
        /* 2. 修改要绑定网卡的配置文件(添加内容) */
        for (size_t i=0; i<NcNameSet.size(); ++i) {
                //组成网络配置文件路径
                string ncPath = NETWORK_DIR + NcNameSet[i];
                //存放从属网卡的信息
                struct _NetworkConfig slaveNcInfo;
                ConfigReader<struct _NetworkConfig> crNC(ncPath);
                ret = crNC.Read(slaveNcInfo);
                if (ret < 0) {
                        LOG_ERR("Config Read Error! path=%s\n", ncPath.c_str());
                        return -3;
                }
                slaveNcInfo.type    = SUBBOND;
                slaveNcInfo.master  = NCInfo.name;
                ret = SetNetworkConfig(NcNameSet[i], slaveNcInfo);
                if (ret < 0) {
                        LOG_ERR("SetNetworkConfig Error! name=%s\n", NcNameSet[i].c_str());
                        return -2;
                }
        }
        return ret;
        /* 4. 重启网络，重启由外层来做 */
}

int NetworkMgr::DelBond(const string& bondName, const vector<string>& bondNCNameSet)
{
        int ret = 0;
        vector<string> tmpVec;
        /* 2. 删除bond[n]文件 */
        ret = DelBondConfig(bondName);
        if (ret < 0) {
                LOG_ERR("DelBondConfig Error! name=%s\n", bondName.c_str());
                return -1;
        }
        /* 3. 修改要绑定网卡的配置文件(删除内容) */
        struct _NetworkConfig NCInfo;
        vector<string>::const_iterator vsIter = bondNCNameSet.begin();
        for (; vsIter != bondNCNameSet.end(); ++vsIter) {
                string ncPath = NETWORK_DIR + *vsIter;
                ConfigReader<struct _NetworkConfig> crNC(ncPath);
                ret = crNC.Read(NCInfo);
                if (ret < 0) {
                        LOG_ERR("Config Read Error! path=%s\n", ncPath.c_str());
                        return -3;
                }
                NCInfo.type = REGULAR;
                ret = crNC.Write(NCInfo);
                if (ret < 0) {
                        LOG_ERR("Config Read Error! path=%s\n", ncPath.c_str());
                        return -4;
                }
        }
        /* 卸载bonding模块 modprobe -r */
        ret = Execute(CL_UNBOND_MOD, tmpVec);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s\n", CL_UNBOND_MOD);
                ret = -7;
        }
        /* 5. 重启网络 */
        return ret;
}

int NetworkMgr::ModifyBond(const struct _NetworkConfig& NCInfo)
{
        int ret = 0;
        ret = SetNetworkConfig(NCInfo.name, NCInfo);
        if (ret < 0) {
                LOG_ERR("SetNetworkConfig Error! name=%s\n", NCInfo.name.c_str());
                return -2;
        }
        return ret;
}

int NetworkMgr::GetAllNetCardName(vector<string>& NCNameSet)
{
        int ret = 0;
        ret = Execute(CL_GET_ALLNCNAME, NCNameSet);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s\n", CL_GET_ALLNCNAME);
        }
        return ret;
}

int NetworkMgr::RestartNetwork()
{
        int ret = 0;
        vector<string> errVec;
        ret = Execute(CL_RESTART_NETWORK, errVec);
        if (ret < 0) {
                string errInfo;
                for (size_t i=0; i<errVec.size(); ++i) {
                        errInfo += errVec[i] + "\n";
                }
                LOG_ERR("Execute Error! cmd=%s, errInfo=%s\n",
                                        CL_RESTART_NETWORK, errInfo.c_str());
        }
        return ret;
}

int NetworkMgr::RestartNetcard(const string& ncName)
{
        int ret = 0;
        vector<string> errVec;
        string cmd = CL_RESTART_NETCARD(ncName);
        ret = Execute(cmd, errVec);
        if (ret < 0) {
                string errInfo;
                for (size_t i=0; i<errVec.size(); ++i) {
                        errInfo += errVec[i] + "\n";
                }
                LOG_ERR("Execute Error! cmd=%s, errInfo=%s\n", 
                                        cmd.c_str(), errInfo.c_str());
        }
        return ret;
}

int NetworkMgr::Ifup(const string& ncName, string& errInfo)
{
        int ret = 0;
        vector<string> errVec;
        string cmd = CL_IFUP_NETCARD(ncName);
        ret = Execute(cmd, errVec);
        if (ret < 0) {
                //string errInfo;
                for (size_t i=0; i<errVec.size(); ++i) {
                        errInfo += errVec[i] + "\n";
                }
                LOG_ERR("Execute Error! cmd=%s, errInfo=%s\n", 
                                        cmd.c_str(), errInfo.c_str());
        }
        return ret;
}

int NetworkMgr::Ifdown(const string& ncName, string& errInfo)
{
        int ret = 0;
        vector<string> errVec;
        string cmd = CL_IFDOWN_NETCARD(ncName);
        ret = Execute(cmd, errVec);
        if (ret < 0) {
                //string errInfo;
                for (size_t i=0; i<errVec.size(); ++i) {
                        errInfo += errVec[i] + "\n";
                }
                LOG_ERR("Execute Error! cmd=%s, errInfo=%s\n", 
                                        cmd.c_str(), errInfo.c_str());
        }
        return ret;
}

int NetworkMgr::CpEthConf(const string& src, const string& dest)
{
        int ret = 0;
        string tsrc, tdest;
        vector<string> errVec;
        if (src.find("_") != string::npos) {
                tdest = NETWORK_DIR + dest;
                tsrc  = src;
        } else {
                tsrc  = NETWORK_DIR + src;
                tdest = dest;
        }

        string cmd = CL_CP(tsrc, tdest);
        ret = Execute(cmd, errVec);
        if (ret < 0) {
                string errInfo;
                for (size_t i=0; i<errVec.size(); ++i) {
                        errInfo += errVec[i] + "\n";
                }
                LOG_ERR("Execute Error! cmd=%s, errInfo=%s\n", 
                                        cmd.c_str(), errInfo.c_str());
        }
        return ret;
}
