#ifndef __CWEBDAV_H__
#define __CWEBDAV_H__

#include "ServiceAccess.h"
#include "Agent/Nas/Base/ConfApache.h"

#define STR_REQUIREUSER "Require user"
#define STR_REQUIREGROUP "Require group"
#define STR_ALLOWFROM   "Allow from"
#define STR_DENYFROM    "Deny from"
#define STR_WEB_RO      "OPTIONS CONNECT GET POST"
#define STR_WEB_RW      "OPTIONS CONNECT GET PUT POST DELETE COPY MOVE LOCK UNLOCK"
#define STR_WEB_WO      "PUT DELETE COPY MOVE LOCK UNLOCK"
#define STR_WEB_DAVON   "Dav"
#define STR_WEB_OPTIONS "Options"
#define STR_WEB_AUTHTYPE "AuthType"
#define STR_WEB_AUTHNAME "AuthName"
#define STR_WEB_AN_VAL  "\"Nas WebDAV Server\""
#define STR_WEB_AUTHUF  "AuthUserFile"
#define STR_WEB_AUTHGF  "AuthGroupFile"
#define STR_WEB_ALLOWOR "AllowOverride"
#define STR_WEB_ORDER   "Order"
#define STR_WEB_ORDERAD "allow,deny"
#define STR_WEB_ORDERDA "deny,allow"
#define STR_NONE        "none"

class CWebdav : public CServiceAccess
{
public:
        CWebdav();
        ~CWebdav();

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

#endif /*__CWEBDAV_H__*/
