#include <vector>
#include <stdio.h>

#include "DdfsLicenseMgr.h"
#include "Include/Types.h"
#include "Include/String.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"

DdfsLicenseMgr::DdfsLicenseMgr()
{
}

DdfsLicenseMgr::~DdfsLicenseMgr()
{
}


int DdfsLicenseMgr::GetLicenseInfo(struct LicenseInfo &licInfo)
{
        vector<string> buf;
        string cmd = CL_DDFS_GET_LIC_INFO;
        if (Execute(cmd, buf) < 0) {
                LOG_ERR("execute cmd error: %s", cmd.c_str());
                return -1;
        }
        licInfo.Version         = buf[0];
        licInfo.Activated       = buf[1];
        licInfo.TimePoStatus    = buf[2];
        licInfo.QuotaPoStatus   = buf[3];
        licInfo.ABSTimePoStatus = buf[4];
        licInfo.AvailTime       = GetTimeMinute(buf[5]);
        licInfo.CurQuota        = GetQuotaKb(buf[6]);
        licInfo.ABSQuota        = GetQuotaKb(buf[7]);
        licInfo.AvailABSTime    = ABSTimeToUtcSec(buf[8]);
        licInfo.CpuId           = buf[9];
        licInfo.Inode           = buf[10];
        licInfo.SerialCode      = buf[11];
        return 0;
}

int DdfsLicenseMgr::ImportLicense()
{
        vector<string> buf;
        string cmd = CL_DDFS_IMPORT_LIC;
        if (Execute(cmd, buf) < 0) {
                LOG_ERR("execute cmd error: %s", cmd.c_str());
                return -1;
        }
        return 0;
}

int DdfsLicenseMgr::ExportLicense()
{
        vector<string> buf;
        string cmd = CL_DDFS_EXPORT_LIC;
        if (Execute(cmd, buf) < 0) {
                LOG_ERR("execute cmd error: %s", cmd.c_str());
                return -1;
        }
        return 0;
}
