#ifndef __NETUSER_H__
#define __NETUSER_H__

#include "Include/Constant.h"

#include <string>
#include <vector>

class CNetUser
{
public:
        CNetUser() {};
        ~CNetUser() {};

        // for all services
        static  int     AddUser(const char* user, const char* pwd = "", const char* group = "");
        static  int     DelUser(const char* user, int nReload = 0);
        static  int     SetPwd(const char* user, const char* pwd = "");

        static  int     AddGroup(const char* group);
        static  int     DelGroup(const char* group, int nReload = 0);

        // for single service
        static  int     AddUserOfRsync(const char* user, const char* pwd = "");
        static  int     DelUserOfRsync(const char* user);
        static  int     SetPwdOfRsync(const char* user, const char* pwd = "");

        static  int     AddUserOfWebdav(const char* user, const char* pwd = "", const char* group = "");
        static  int     DelUserOfWebdav(const char* user);
        static  int     SetPwdOfWebdav(const char* user, const char* pwd = "");

        static  int     AddGroupOfWebdav(const char* group);
        static  int     DelGroupOfWebdav(const char* group);

};

#endif /*__NETUSER_H__*/

