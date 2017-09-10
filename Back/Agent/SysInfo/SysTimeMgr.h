#ifndef _SYS_TIMER_MGR_H_
#define _SYS_TIMER_MGR_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

class SysTimeMgr {
public:
        SysTimeMgr();
        ~SysTimeMgr();

public:
        int SetZone(const string &zone);
        int SetTime(const string& timeStr);
        int SetNtpServer(const string& ip);
        int SyncTime(const string& ip);
        int GetTimeInfo(struct TimeInfo &info);

        int GetZone(string &zone);
        int GetTime(string& timeStr);
        int GetNtpServer(string& ip);

private:
        // static vector<string> m_Zone;
        // vector<string> m_Zone;
};

#endif //_SYS_TIMER_MGR_H_
