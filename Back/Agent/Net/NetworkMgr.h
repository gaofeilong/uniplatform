#ifndef _NETWORK_MGR_H_
#define _NETWORK_MGR_H_

#include "Public/Config/ConfigReader.h"

#include <map>
#include <set>
#include <vector>

using std::map;
using std::set;
using std::vector;

class NetworkMgr {
public:
        NetworkMgr();
        ~NetworkMgr();

public:
        int GetDns(vector<string>& dns);
        int SetDns(const vector<string> &dns);
        int GetHostName(string &host);
        int SetHostName(const string &host);

        int GetNetCardSet(map<string, struct _NetworkConfig>& NCSet);
        int GetNetworkConfig(const string& name, struct _NetworkConfig& NCInfo);
        int GetBuildBondNC(const string& bondName, vector<string>& bondNCNameSet);

        int SetNetworkConfig(const string& name, const struct _NetworkConfig& NCInfo);

        int DelBond(const string& bondName, const vector<string>& bondNCNameSet);
        int ModifyBond(const struct _NetworkConfig& NCInfo);
        int AddBond(const vector<string>& NcNameSet, const struct _NetworkConfig& NCInfo);

        int RestartNetwork();
        int RestartNetcard(const string& ncName);

        int Ifup(const string& ncName, string& errInfo);
        int Ifdown(const string& ncName, string& errInfo);
        int CpEthConf(const string& src, const string& dest);
private:
        int DelBondConfig(const string& name);
        int GetAllNetCardName(vector<string>& NCNameSet);
};

#endif //_NETWORK_MGR_H_
