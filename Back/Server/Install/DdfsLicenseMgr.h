#ifndef _DDFS_LICENSE_MGR_H_
#define _DDFS_LICENSE_MGR_H_
#include "LicenseMgr.h"

class DdfsLicenseMgr: public LicenseMgr {
public:
        explicit DdfsLicenseMgr();
        ~DdfsLicenseMgr();

public:
        int GetLicenseInfo(struct LicenseInfo &licInfo);
        int ImportLicense();
        int ExportLicense();
};

#endif //_DDFS_LICENSE_MGR_H_
