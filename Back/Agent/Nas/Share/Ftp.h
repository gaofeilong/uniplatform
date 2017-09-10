#ifndef __CFTP_H__
#define __CFTP_H__

#include "ServiceAccess.h"
#include "Agent/Nas/Base/ConfApache.h"

#define STR_ALLOWUSER   "AllowUser"
#define STR_ALLOWGROUP  "AllowGroup"
#define STR_ALLOWFROM   "Allow from"
#define STR_FTP_RO      "READ"
#define STR_FTP_WO      "WRITE"
#define STR_FTP_RW      "READ WRITE"
#define STR_FTP_DIRS    "DIRS"
#define STR_DENYALL     "DenyAll"
#define STR_NONE        "none"

class CFtp : public CServiceAccess
{
public:
        CFtp();
        ~CFtp();

        int     AddDir(const char* path, uint perm = SA_RO);
        int     DelDir(const char* path);
        int     GetDir(const char* path, uint& perm);
        int     SetDir(const char* path, uint perm);

        int     GetUserAccess(const char* path, std::vector<std::string>& va);
        int     SetUserAccess(const char* path, const std::vector<std::string>& va);

        int     GetIpAccess(const char* path, std::vector<std::string>& va);
        int     SetIpAccess(const char* path, const std::vector<std::string>& va);

        static int      RemoveUser(const std::vector<std::string>& vu);
        static int      RemoveIp  (const std::vector<std::string>& vip);

        int     LoadIn();
private:
        CConfApache     m_conf;
};

#endif /*__CFTP_H__*/
