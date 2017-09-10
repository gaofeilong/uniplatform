#ifndef _INSTALL_MGR_H_
#define _INSTALL_MGR_H_

#include <vector>
#include <string>
#include "Public/Mysql/Mysql.h"
#include "Utils/Json/include/json.h"

using std::vector;
using std::string;

class InstallMgr {
public:
        InstallMgr();
        ~InstallMgr();

public:
        int ConnectIndexNode(const string &user, const string &password, const string &ip);
        int ConnectDataNode(const string &user, const string &password, const vector<string> &ipList, const string &indexId);
        int InstallIndexNode(const string &user, const string &password, const string &ip);
        int InstallDataNode(const string &user, const string &password, const vector<string> &ipList, const string &indexId);
        int RemoveIndexNode(const string &user, const string &password, const string &ip);
        int RemoveDataNode(const string &user, const string &password, const vector<string> &ipList, const string &indexId);
private:
        int StartConnect(const string &user, const string &password, 
                        const vector<string> &ipList, const string &type, const string &indexId);
        int StartInstallIndex(const string &user, const string &password, const string ip);
        int StartInstallData(const string &user, const string &password, const vector<string> &ipList,const string &indexId);
        int StartRemoveIndex(const string &user, const string &password, const string &ip);
        int StartRemoveData(const string &user, const string &password, const vector<string> &ipList, const string &indexId);
        int InstallGatewayAgent(MysqlClient **mysql, const string &user, const string password, const string &ip, const string &ms);
        int InstallStep(MysqlClient **mysql, const string &ip, const string &cmd, const string &process, const string &desc);
        int UninstallStep(MysqlClient **mysql, const string &ip, const string &cmd, const string &process, const string &desc);
        int GetServerPortIp(string &serverIp);
};

#endif //_INSTALL_MGR_H_
