#ifndef _ARCHIVE_JSON_MGR_H_
#define _ARCHIVE_JSON_MGR_H_

#include <string>
#include <stdint.h>
#include "Agent/Frame/IJsonMgr.h"

using std::string;

class ArchiveJsonMgr : public IJsonMgr {
public: 
        ArchiveJsonMgr();
        ~ArchiveJsonMgr();

public:
        Packet *Handle(const Packet *p);

private:
        Packet *AddArchiveTask(const Packet *p);
        Packet *DelArchiveTask(const Packet *p);
        Packet *ModifyArchiveTask(const Packet *p);
        Packet *StartArchiveTask(const Packet *p);
        Packet *StopArchiveTask(const Packet *p);
        Packet *StartArchiveService(const Packet *p);
        Packet *StopArchiveService(const Packet *p);
        Packet *LookupArchiveInfo(const Packet *p);
        Packet *LookupArchiveList(const Packet *p);

private:
        int64_t Htom(const string& time);
        int64_t GetRun(const string& start, const string& end);
        string  CronString(const string& time, const string& interval);

};

#endif //_ARCHIVE_JSON_MGR_H_
