#include <vector>
#include <stdio.h>

#include "IstriLicenseMgr.h"
#include "Include/Types.h"
#include "Include/String.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"

IstriLicenseMgr::IstriLicenseMgr(const string &ip, const string &user, const string &password):
        m_Ip(ip), m_User(user), m_Password(password)
{
}

IstriLicenseMgr::~IstriLicenseMgr()
{
}


int IstriLicenseMgr::GetLicenseInfo(struct LicenseInfo &licInfo)
{
        vector<string> buf;
        string cmd = CL_FBS_GET_LIC_INFO(m_Ip, m_User, m_Password);
        if (Execute(cmd, buf) < 0) {
                LOG_ERR("execute cmd %s error: %s", cmd.c_str(), buf.back().c_str());
                return -1;
        }
        licInfo.Version         = buf[0];
        licInfo.Activated       = buf[1];
        licInfo.NodeNumber      = buf[2];       // for istri    
        licInfo.CurNodeNumber   = buf[3];       // for istri
        licInfo.TimePoStatus    = buf[4];
        licInfo.QuotaPoStatus   = buf[5];
        licInfo.ABSTimePoStatus = buf[6];
        licInfo.AvailTime       = GetTimeMinute(buf[7]);
        licInfo.CurQuota        = GetQuotaKb(buf[8]);
        licInfo.ABSQuota        = GetQuotaKb(buf[9]);
        licInfo.AvailABSTime    = ABSTimeToUtcSec(buf[10]);
        licInfo.CpuId           = buf[11];
        licInfo.Inode           = buf[12];
        licInfo.SerialCode      = buf[13];
        return 0;
}

int IstriLicenseMgr::CheckLicense()
{
        int ret = 0;
        struct LicenseInfo licInfo;
        if (GetLicenseInfo(licInfo) < 0) {
                LOG_ERR("get license info error");
                return -1;
        }

        if (licInfo.Activated == "y") {
                return 0;
        }
        if (licInfo.QuotaPoStatus == "y" && 
                (double)licInfo.CurQuota / (double)licInfo.ABSQuota > 0.8) {
                ret |= QUOTA_INVALID;
        }
        if (licInfo.TimePoStatus == "y" && licInfo.AvailTime <= MINUTE_PER_MONTH) {
                ret |= TIME_INVALID;
        }
        if (licInfo.ABSTimePoStatus == "y" && 
                time(NULL) + SEC_PER_MONTH >= licInfo.AvailABSTime) {
                ret |= ABSTIME_INVALID;
        }
        return ret;
}

int IstriLicenseMgr::ImportLicense()
{
        vector<string> buf;
        string cmd = CL_FBS_IMPORT_LIC(m_Ip, m_User, m_Password);
        if (Execute(cmd, buf) < 0) {
                LOG_ERR("execute cmd %s error: %s", cmd.c_str(), buf.back().c_str());
                return -1;
        }
        return 0;
}

int IstriLicenseMgr::ExportLicense()
{
        vector<string> buf;
        string cmd = CL_FBS_EXPORT_LIC(m_Ip, m_User, m_Password);
        if (Execute(cmd, buf) < 0) {
                LOG_ERR("execute cmd %s error: %s", cmd.c_str(), buf.back().c_str());
                return -1;
        }
        return 0;
}
