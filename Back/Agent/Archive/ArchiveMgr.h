#ifndef _ARCHIVE_MGR_H_
#define _ARCHIVE_MGR_H_

#include "Public/Config/ConfigReader.h"
//#include "Utils/Mysql/MysqlClient.h"

class ArchiveMgr {
public:
        ArchiveMgr();
        ~ArchiveMgr();

public:
        int AddArchiveTask(struct _ArchiveConfig& archiveConfig);
        int DelArchiveTask(const string &id);
        int ModifyArchiveTask(const string &id, 
                        const struct _ArchiveConfig& archiveConfig);
        int LookupArchiveInfo(const string &id, 
                        struct _ArchiveConfig& archiveConfig);

        int SetArchiveState(const string& id, const string& nodeId, const string& state);
        int GetArchiveState(const string& id, const string& nodeId, string& state);
        // int LookupArchiveTaskList(map<int, string>& archiveTaskList);
        // int StartArchiveTask(int configId);
        // int StopArchiveTask(int configId);
        // int StartArchiveService(int configId);
        // int StopArchiveService(int configId);

private:
        int GetMaxId(int& maxId);
};

#endif //_ARCHIVE_MGR_H_
