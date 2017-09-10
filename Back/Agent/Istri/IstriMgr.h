#ifndef _ISTRI_MGR_H_
#define _ISTRI_MGR_H_

#include <map>
#include <string>
#include <vector>
#include "Include/Types.h"
#include "Server/Install/LicenseMgr.h"
using std::map;
using std::string;
using std::vector;
using std::make_pair;

struct NodeState {
        NodeState(const string &ip, time_t time, const string &state, const string &table): 
                  Ip(ip), Time(time), State(state), Table(table) {}
        string Ip;
        time_t Time;
        string State;
        string Table;
};

class IstriMgr {
public:
        IstriMgr();
        ~IstriMgr();
public:
        int StartIstriIndex(string &errInfo);
        int StartIstriData(string &errInfo);
        int StopIstriIndex(string &errInfo);
        int StopIstriData(string &errInfo);
        int FixIstriIndex(string &errInfo);
        int FixIstriData(string &errInfo);
        int EditIstriIndex(const vector<string> &oldIdxRootPath, const vector<string> &newIdxRootPath);
        int FirstEditIstriData(const string &mp, const string &indexPath, const string &metaPath, 
                                        const vector<string> &dataPath, const string &isImgDedup);
        int EditIstriData(const vector<string> &oldDataPath, const vector<string> &newDataPath);
        int EditIstriMultiPath(const vector<string> &oldPath, const vector<string> &newPath);
        int ClearIstriDataId(const string &id, string &errInfo);

public:
        int UpdateNodeInfo();
        int UpdateNodeState();
private:
        int UpdateIndexInfo(); 
        int UpdateDataInfo(); 
        int UpdateDdfsInfo(); 
        bool InitPath(const string &path);
        string DataServiceStatus();
        string IndexServiceStatus();
        string ServiceStatus(const string &cmd);
        // int GetGatewayConfig(struct _GatewayConfig &c);
        int InitNodeState(map<string, struct NodeState> &nodeStateList);
        int SetNodeStateLost(const string &ip, const string &table);
        int CheckLicense(BOOSTPTR<LicenseMgr> &lic, const string &info, int &state);
};

#endif  //_ISTRI_MGR_H_
