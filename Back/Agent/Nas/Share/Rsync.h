#ifndef __CRSYNC_H__
#define __CRSYNC_H__

#include "ServiceAccess.h"
#include "Agent/Nas/Base/ConfParser.h"

#define STR_AUTHUSERS   "auth users"
#define STR_HOSTSALLOW  "hosts allow"
#define STR_READONLY    "read only"
#define STR_WRITEONLY   "write only"

class CRsync : public CServiceAccess
{
public:
        CRsync();
        ~CRsync();

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
        CConfParser     m_conf;
};

#endif /*__CRSYNC_H__*/
