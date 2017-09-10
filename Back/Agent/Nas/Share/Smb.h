#ifndef __DEALSMB_H__
#define __DEALSMB_H__

#include <string>
#include <vector>
#include <algorithm>
//#include "../DealService/ServiceMgr.h"
#include "ServiceAccess.h"
#include "Agent/Nas/Base/ConfParser.h"

#define STR_WRITABLE    "writable"
#define STR_BROWSABLE   "browsable"
#define STR_CREATEMODE  "create mode"
#define STR_DIRMODE     "directory mode"
#define STR_DENYHOSTS   "deny hosts"
#define STR_ALLOWHOSTS  "allow hosts"
#define STR_VALIDUSERS  "valid users"
#define STR_INVALIDUSERS "invalid users"
#define STR_READLIST    "read list"
#define STR_WRITELIST   "write list"
#define STR_MAPTOGUEST  "map to guest"
#define STR_BADUSER     "bad user"
#define STR_GUESTACCT   "guest account"
#define STR_GUESTOK     "guest ok"

class CSmb : public CServiceAccess
{
public:
        CSmb();
        ~CSmb();

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

        int     GetAnonyStat(const char* path, uint& stat);
        int     SetAnonyStat(const char* path, uint stat);

// 服务控制
        int     LoadIn();
        void    GetADdomain(int& usertype, std::string& short_domain);

private:
        CConfParser     m_conf;
};

#endif
