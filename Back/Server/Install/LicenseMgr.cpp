#include <vector>
#include <stdio.h>

#include "LicenseMgr.h"
#include "Include/Types.h"
#include "Include/String.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"

LicenseMgr::LicenseMgr()
{
}

LicenseMgr::~LicenseMgr()
{
}

int LicenseMgr::CheckLicense()
{
        int ret = 0;
        struct LicenseInfo licInfo;
        if (GetLicenseInfo(licInfo) < 0) {
                LOG_ERR("get license info error");
                return -1;
        }

        // if not activated, check time and quota, else check quota only
        if (licInfo.Activated != "y") {
                if (licInfo.TimePoStatus == "y" && 
                                licInfo.AvailTime <= MINUTE_PER_MONTH) {
                        ret |= TIME_INVALID;
                }
                if (licInfo.ABSTimePoStatus == "y" && 
                        time(NULL) + SEC_PER_MONTH >= licInfo.AvailABSTime) {
                        ret |= ABSTIME_INVALID;
                }
        }
        if (licInfo.QuotaPoStatus == "y" && 
                (double)licInfo.CurQuota / (double)licInfo.ABSQuota > 0.8) {
                ret |= QUOTA_INVALID;
        }

        return ret;
}

size_t LicenseMgr::GetTimeMinute(const string &strTime)
{
        // "89.98 day" or "89.98 minute"
        if (strTime.find("day") != string::npos) {
                return atof(strTime.substr(0, strTime.size() - 4).c_str()) * MINUTE_PER_DAY;
        } else if (strTime.find("minute") != string::npos) {
                return atol(strTime.substr(0, strTime.size() - 7).c_str());
        }
        return 0;
}

time_t LicenseMgr::ABSTimeToUtcSec(const string &strTime)
{
        // "2014-10-08-15:59:59"
        struct tm t;
        strptime(strTime.c_str(), "%Y-%m-%d-%H-%M-%S", &t);
        return mktime(&t);
}

size_t LicenseMgr::GetQuotaKb(const string &strQuota)
{
        // "89.98 KB" or "89.98 MB or ..."
        if (strQuota.find("KB") != string::npos) {
                return atof(strQuota.substr(0, strQuota.size() - 3).c_str());
        } else if (strQuota.find("MB") != string::npos) {
                return (atof(strQuota.substr(0, strQuota.size() - 3).c_str()) * 1024);
        } else if (strQuota.find("GB") != string::npos) {
                return (atof(strQuota.substr(0, strQuota.size() - 3).c_str()) * 1024 * 1024);
        } else if (strQuota.find("TB") != string::npos) {
                return (atof(strQuota.substr(0, strQuota.size() - 3).c_str()) * 1024 * 1024 * 1024);
        } else if (strQuota.find("PB") != string::npos) {
                return (atof(strQuota.substr(0, strQuota.size() - 3).c_str()) * 1024 * 1024 * 1024 * 1024);
        } else if (strQuota.find("EB") != string::npos) {
                return (atof(strQuota.substr(0, strQuota.size() - 3).c_str()) * 1024 * 1024 * 1024 * 1024 * 1024);
        }
        return 0;
}
