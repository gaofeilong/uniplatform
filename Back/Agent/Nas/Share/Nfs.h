#ifndef __DEALNFS_H__
#define __DEALNFS_H__

#include <string>
#include <vector>
#include <algorithm>

//#include "../DealService/ServiceMgr.h"
#include "ServiceAccess.h"

typedef struct stNetAccessIp
{
        std::string     name;
        std::string     ip;
        std::string     mask;
        std::string     sharetype;
        uint            masklen;
} stNetAccessIp;

typedef struct stNfsIp
{
        stNetAccessIp nai;      // nfs 中 masklen 比 mask 优先级高
        int          perm;      // nfs 权限: 0=no 1=ro 2=rw +0x10=no_root_squash <0x10=all_squash
        int        squash;
        int       anonuid;
        int       anongid;
        int        secure;
        int        wdelay;
        int        iomode;
        // 生成网段选项字符串(不包含地址/掩码,带括号), squash 由 perm 决定
        const char* OptStr(uint fsid) const;
        stNfsIp() :perm(SA_RO),squash(NFS_ALLSQUASH),anonuid(NFS_ANONUID),
               anongid(NFS_ANONGID),secure(NFS_SECURE),wdelay(NFS_WDELAY),iomode(NFS_SYNC) {}
} stNfsIp;

typedef struct stNfsDir
{
        std::string          path;
        std::vector<stNfsIp> vnis;
        stNfsDir() {}
        stNfsDir(std::string p) :path(p) {}
} stNfsDir;


class CNfs : public CServiceAccess
{
public:
        CNfs();
        ~CNfs();

        int     AddDir(const char* path, uint perm = SA_RO);
        int     DelDir(const char* path);
        int     GetDir(const char* path, uint& perm);
        int     SetDir(const char* path, uint perm);

        int     GetUserAccess(const char* path, std::vector<std::string>& va);
        int     SetUserAccess(const char* path, const std::vector<std::string>& va);

        int     GetIpAccess(const char* path, std::vector<std::string>& va);
        int     SetIpAccess(const char* path, const std::vector<std::string>& va);

        int     RemoveUser(const std::vector<std::string>& vu);
        int     RemoveIp  (const std::vector<std::string>& vip);

        int     GetIpAccess(const char* path, std::vector<stNfsIp>& va);
        int     SetIpAccess(const char* path, const std::vector<stNfsIp>& va);

        void    SetPerm(int perm);
        uint    GetPerm() const;
// 服务操作
        int     LoadIn();

// 文件操作
        int     Load();
        int     Write() const;
        int     Clear();

// 其他操作
        uint InsertIntoVector(std::vector<stNfsIp>& vnis, const stNfsIp& stni);

private:
        std::string             m_filepath;
        std::vector<stNfsDir>   m_vnd;
        int                     m_ready;
        int                     m_perm;
};

#endif
