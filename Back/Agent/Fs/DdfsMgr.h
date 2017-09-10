#ifndef _DDFS_MGR_H_
#define _DDFS_MGR_H_

#include "FsMgr.h"
#include "DdfsInfo.h"
#include "Public/Config/ConfigStruct.h"

#include <vector>

using std::vector;

class DdfsMgr: public FsMgr {
public:
        DdfsMgr();
        ~DdfsMgr();
public:
        string GetMpStatus(const string &mp);
        int GetMpList(vector<struct _DdfsPair>& mpList);
        int GetConfigByMp(const string& configPath, string& mp);
        int GetDedupInfo(const string& mp, struct _DedupInfo &dedupInfo);
        int LookConf(const string& configPath, struct _DdfsConfig& ddfsConfig);

        int Mkfs(const string& configPath, vector<string>& errVec);
        int Mount(const struct _DdfsPair& ddfsPair, vector<string>& errVec);
        int Umount(const struct _DdfsPair& ddfsPair, vector<string>& errVec);
        int Create(const struct _DdfsConfig& ddfsConfig, vector<string>& errVec);
        int Fsck(const string& mountPoint, const string& configPath, vector<string>& errVec);
        int Modify(const string& configPath, const vector<string> allPathList, vector<string>& errVec);
        int Remove(const string& configPath);

private:
        int GetArchiveStateByDest(const string& mp);
        int GetNodeIdByIp(const string& ip, string& id);
};

#endif  //_DDFS_MGR_H_
