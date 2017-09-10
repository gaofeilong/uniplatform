#ifndef __FS_OPER_H__
#define __FS_OPER_H__

#include <string>
#include <vector>

// for GetLvSql()'s param LVPATH
#define LVPATH_ALL      "*"
#define LVPATH_FREE     "#"
#define LVPATH_PREMOUNT "%"
#define LVMOUNTIS(m)    (std::string(LVPATH_PREMOUNT)+m)
// for SetLvSql()'s param DOING
#define LVSQL_UNUSED    0
#define LVSQL_USED      1
#define LVSQL_DEL       -1
#define LVSQL_ADD       2
#define LVSQL_SETSIZE   3
#define LVSQL_SETFS     4
#define LVSQL_FREEFS    40
// for `lvtype`'s column ISUSED
#define LVUSED_NONE     0
#define LVUSED_ISCSI    1
#define LVUSED_FC       2
#define LVUSED_SHARE    3
#define LVUSED_DDFS     4

static const char* LVUSSTR[10] = {"", "iscsi", "fc", "share", "ddfs", NULL};

typedef struct stLvInfo
{
        std::string path;
        std::string name;
        std::string vgname;
        std::string mount;
        std::string fsfmt;
        unsigned long size;
        int used;
        stLvInfo():size(0), used(0) {}
        stLvInfo(std::string p, std::string m = "", std::string f = "")
                :path(p),mount(m),fsfmt(f),size(0),used(0) {fmt();}
        stLvInfo(std::string p, std::string m, int u)
                :path(p),mount(m),size(0),used(u)
        {
                fmt();
                if (used > 0) fsfmt = LVUSSTR[used%10];
        }
        stLvInfo(std::string p, std::string m, std::string f, int s, int u)
                :path(p),mount(m),fsfmt(f),size(s),used(u)
        {
                fmt();
                if (used > 0) fsfmt = LVUSSTR[used%10];
        }
        void fmt();
} stLvInfo;

namespace FsOper
{
//public:

        enum FS_INDEX {
                FS_MIN = 0,
                FS_EXT2,
                FS_EXT3,
                FS_EXT4,
                FS_MAX
        };

        // 挂载操作
        bool MountLv(const std::string& srcPath, const std::string& destPath,
                          int fsType, const char *opt = NULL);
        // 卸载操作
        bool UmountLv(const std::string& path);
        // 重新挂载， 可以指定选项
        bool Remount(const std::string & path, char *opt = NULL);
        // 格式化某设备
        bool Format(const std::string& path, int fsType, const char *fsOpt = NULL);
        // 调整某设备size
        bool ResizeFs(const std::string& path);

        // copy 文件至目的
        bool Cp(const std::string& srcPath, const std::string& dstPath);
        // 创建文件夹 mkdir -p
        bool MkDir(const std::string & path);
        // 删除文件夹 rm -rf
        bool RmDir(const std::string & path);

        // quotacheck 挂载点路径
        bool QuotaCheck(const std::string & path);
        // quota on
        bool QuotaOn(const std::string& mtPath);

        // 添加 quota 信息至 /etc/fstab
        //static bool AddQuota2Tab(const std::string& lvPath, const std::string& mtPath, const char* fsType);
        // 删除quota信息 /etc/fstab
        //static bool RemQuota2Tab(const std::string& lvPath);

        // add mount info to fstab
        bool AddToFstab(const std::string& lvPath, const std::string& mtPath,
                               const char* fsType, const char *opt = NULL);
        // rm mount info from fstab
        bool RmToFstab(const std::string& lvPath);

        // lvpath 的数据库记录 *代表获取所有 #代表free @代表iscsi $代表fc
        // doing:{1=used; 0=unused; -1=delete; 2=add; 3=set size; 4=set fs; 40=set fs=free;}
        int GetLvSql(std::vector<stLvInfo>& vl, const std::string lvpath = LVPATH_ALL);
        int SetLvSql(const stLvInfo& li, int doing);
}

#endif /* __FS_OPER_H__ */
