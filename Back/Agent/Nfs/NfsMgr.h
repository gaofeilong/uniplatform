/*******************************************************************************
 *
 * @ File Name  : NfsMgr.h
 * @ Date       : 2013-10-21
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: NFS 模块功能处理
 * @ History    : 2013-10-21：创建
 *
 * ****************************************************************************/

#ifndef _NFS_MGR_H_
#define _NFS_MGR_H_
#include <string>
#include <vector>
using std::string;
using std::vector;

struct NfsClientInfo {
        string ip;
        string password;
        string state;
};

struct NfsServerInfo {
        string name;
        string path;
        string mp;
        string mpState;
        string state;
        string parameter;
        vector<struct NfsClientInfo> clientList;
};

class NfsMgr {
public:
        NfsMgr();
        ~NfsMgr();
public:
        int GetInfo(vector<struct NfsServerInfo> &nfsInfoList);
        int Create(const struct NfsServerInfo &nfsServerInfo, vector<string> &errIp);
        int Edit(const struct NfsServerInfo &nfsServerInfo, vector<string> &errIp);
        int Remove(const string &name);
        int Start();
        int Stop();
        int MountClient(const string &name, const string &ip);
        int UmountClient(const string &name, const string &ip);
        int RemoveClient(const string &name, const string &ip);

private:
        int RemoveNfsConfItem(const string &item);
        int AddNfsConfItem(const string &item);
        int ReplaceNfsConfItem(const string oldItem, const string &newItem);
        int GetMpType(const string &mp, string &type);
};

#endif  //_NFS_MGR_H_
