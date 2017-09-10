#ifndef _ISTRI_LICENSE_MGR_H_
#define _ISTRI_LICENSE_MGR_H_
#include "LicenseMgr.h"

class IstriLicenseMgr: public LicenseMgr {
public:
        explicit IstriLicenseMgr(const string &ip, const string &user, const string &password);
        ~IstriLicenseMgr();

public:
        int GetLicenseInfo(struct LicenseInfo &licInfo);
        int CheckLicense();
        int ImportLicense();
        int ExportLicense();
private:
        string m_Ip;
        string m_User;
        string m_Password;
};

#endif //_ISTRI_LICENSE_MGR_H_
