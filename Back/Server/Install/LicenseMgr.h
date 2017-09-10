#ifndef _LICENSE_MGR_H_
#define _LICENSE_MGR_H_

#include <string>
using std::string;

#define QUOTA_INVALID           (1<<0)
#define TIME_INVALID            (1<<1)
#define ABSTIME_INVALID         (1<<2)


struct LicenseInfo {
        string Version;
        string Activated;
        string NodeNumber;
        string CurNodeNumber;
        string TimePoStatus;
        string QuotaPoStatus;
        string ABSTimePoStatus;
        size_t AvailTime;
        double CurQuota;
        double ABSQuota;
        time_t AvailABSTime;
        string CpuId;
        string Inode;
        string SerialCode;
};

class LicenseMgr {
public:
        LicenseMgr();
        virtual ~LicenseMgr();

public:
        virtual int GetLicenseInfo(struct LicenseInfo &licInfo) = 0;
        virtual int ImportLicense() = 0;
        virtual int ExportLicense() = 0;
        virtual int CheckLicense();

protected:
        size_t GetTimeMinute(const string &strTime);
        time_t ABSTimeToUtcSec(const string &strTime);
        size_t GetQuotaKb(const string &strQuota);
};

#endif //_LICENSE_MGR_H_
