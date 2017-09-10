#ifndef __SERVICEACCESS_H__
#define __SERVICEACCESS_H__

#include "Include/Constant.h"
//#include "Utils/Log/Log.h"
//#include "Utils/Common/CommonFunc.h"
//#include "Agent/Nas/Base/ConfParser.h"
//#include "Agent/Nas/Base/ConfApache.h"
#include <string>
#include <vector>

#define SHNAME_ALL      "*"
#define SHNAME_FREE     "#"
//#define LVPATH_PREMOUNT "%"
//#define LVMOUNTIS(m)    (std::string(LVPATH_PREMOUNT)+m)
// for SetLvSql()'s param DOING
#define SHSQL_ADD       2
#define SHSQL_DEL       -1
#define SHSQL_SETMASK   3
// for `lvtype`'s column MASK
#define MSK_SMB         0x0001
#define MSK_NFS         0x0002
#define MSK_RSYNC       0x0004
#define MSK_FTP         0x0008
#define MSK_DAV         0x0010
#define MSK_AFP         0x0020
#define MSK_DDFS        0x1000

typedef struct stShare
{
        std::string     name;
        std::string     lv;
        uint            mask;
        stShare() :mask(0) {}
        stShare(std::string n, std::string l, uint m = 0) :name(n), lv(l), mask(m) {}
        stShare(std::string n, uint m = 0) :name(n), mask(m) {}
} stShare;

class CServiceAccess
{
public:
        CServiceAccess(){};
        virtual ~CServiceAccess() {};

        virtual int     AddDir(const char* path, uint perm) = 0;
        virtual int     DelDir(const char* path) = 0;
        virtual int     GetDir(const char* path, uint& perm) = 0;
        virtual int     SetDir(const char* path, uint perm) = 0;

        virtual int     GetUserAccess(const char* path, std::vector<std::string>& va) = 0;
        virtual int     SetUserAccess(const char* path, const std::vector<std::string>& va) = 0;

        virtual int     GetIpAccess(const char* path, std::vector<std::string>& va) = 0;
        virtual int     SetIpAccess(const char* path, const std::vector<std::string>& va) = 0;

        static int      RemoveUser(const std::vector<std::string>& vu, int nReload = 1);
        static int      RemoveGroup(const std::vector<std::string>& vg, int nReload = 1);
        static int      RemoveIp  (const std::vector<std::string>& vip, int nReload = 1);


        virtual int     LoadIn() = 0;
        static void     LoadAll();

        static int      ListShare();
        static int      AddShare();
        static int      DelShare();
        static int      GetShare();
        static int      SetShare();
        static int      GetShSql(std::vector<stShare>&vd, std::string name = "*");
        static int      SetShSql(const stShare& sh, int doing);
protected:
        static std::vector<stShare> m_vd;
};

#endif /*__SERVICEACCESS_H__*/

