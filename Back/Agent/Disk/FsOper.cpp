#include "FsOper.h"
#include "LvmMgr.h"

#include "Utils/Shell/System.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include "Public/Mysql/Mysql.h"
#include "Utils/Shell/ShellOpr.h"
#include "Include/LvmConf.h"

#include <unistd.h>



//
// CFsOper
//
const char* FS_STR[] = {NULL, "ext2", "ext3", "ext4", NULL};
//extern const char* LVUSSTR[] = {"", "iscsi", "fc", "share", "ddfs", NULL, NULL, NULL};

void stLvInfo::fmt()
{
        if (path.empty())
        {
                return ;
        }
        std::vector<std::string> vname;
        Split(path.c_str(), vname, '/');
        if (vname.size() < 4)
        {
                return ;
        }
        name = vname[3];
        vgname = vname[2];
        //if (!mount.empty())
        //{
                return ;
        //}
        //mount = MOUNT_PATH + vgname + "/" + name;//MakeMountDir();
}

namespace FsOper{
// 挂载操作
bool MountLv(const std::string& srcPath, const std::string& destPath, int fsType, const char *opt)
{
        char* cmd = NULL;
        bool  bRet = false;

        if (!opt || !*opt) {
                cmd = s_printf("mount -t %s %s %s", FS_STR[fsType], srcPath.c_str(), destPath.c_str());
        }
        else {
                cmd = s_printf("mount -t %s -o %s %s %s", FS_STR[fsType], opt, srcPath.c_str(), destPath.c_str());
        }

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

// 卸载操作
bool UmountLv(const std::string& path)
{
        char* cmd = NULL;
        bool  bRet = false;

        cmd = s_printf("umount -f %s", path.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

// 格式化某设备
bool Format(const std::string& path, int fsType, const char *fsOpt)
{
        char* cmd = NULL;
        bool  bRet = false;

        if (!fsOpt || !*fsOpt) {
                cmd = s_printf("mkfs.%s %s", FS_STR[fsType], path.c_str());
        }
        else {
                cmd = s_printf("mkfs.%s %s %s", FS_STR[fsType], fsOpt, path.c_str());
        }

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

// 调整某设备size, 内核支持前提下，仅支持ext3,ext4,
bool ResizeFs(const std::string& path)
{
        char* cmd = NULL;
        bool bRet = false;
        cmd = s_printf("resize4fs %s", path.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool Cp(const std::string& srcPath, const std::string& dstPath)
{
        char* cmd = NULL;
        bool bRet = false;
        cmd = s_printf("cp -f %s %s", srcPath.c_str(), dstPath.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool MkDir(const std::string & path)
{
        char* cmd = NULL;
        bool bRet = false;
        cmd = s_printf("mkdir -p %s", path.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool RmDir(const std::string & path)
{
        char* cmd = NULL;
        bool bRet = false;
        cmd = s_printf("rm -rf %s", path.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool Remount(const std::string & path, char *opt)
{
        char* cmd = NULL;
        bool bRet = false;

        if (!opt || !*opt) {
                cmd = s_printf("mount -o remount %s", path.c_str());
        }
        else {
                cmd = s_printf("mount -o remount,%s %s", opt, path.c_str());
        }

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool QuotaCheck(const std::string & path)
{
        char* cmd = NULL;
        bool bRet = false;
        cmd = s_printf("quotacheck -cmug %s", path.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool QuotaOn(const std::string& mtPath)
{
        char* cmd = NULL;
        bool bRet = false;
        cmd = s_printf("quotaon %s", mtPath.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

// bool AddQuota2Tab(const std::string& lvPath, const std::string& mtPath, const char* fsType)
// {
//         char* cmd = NULL;
//         bool  bRet = false;

//         cmd = s_printf("sed -i '$a %s %s %s defaults,usrquota,grpquota 0 0' %s",
//                        lvPath.c_str(), mtPath.c_str(), fsType, CONF_FSTAB);

//         if (cmd == NULL) {
//                 LOG_ERR("s_printf error");
//         }
//         else if (!ExeCmd(cmd)) {
//                 LOG_ERR("Execmd (%s) error", cmd);
//         }
//         else {
//                 bRet = true;
//         }

//         free(cmd);
//         return bRet;
// }

// bool RemQuota2Tab(const std::string& lvPath)
// {
//         char* cmd = NULL;
//         bool  bRet = false;

//         cmd = s_printf("sed -i -e 's_^%s_#_' -e '/^#/d' %s", lvPath.c_str(), CONF_FSTAB);

//         if (cmd == NULL) {
//                 LOG_ERR("s_printf error");
//         }
//         else if (!ExeCmd(cmd)) {
//                 LOG_ERR("Execmd (%s) error", cmd);
//         }
//         else {
//                 bRet = true;
//         }

//         free(cmd);
//         return bRet;
// }

// fstab: 1.path; 2.mount_point; 3.fstype; 4.mount_opt; 5.fs_req; 6.fs_passno
bool AddToFstab(const std::string& lvPath, const std::string& mtPath, const char* fsType, const char *opt)
{
        char* cmd = NULL;
        bool  bRet = false;

        if (!opt || !*opt) {
                cmd = s_printf("sed -i '$a %s %s %s defaults 0 0' %s",
                               lvPath.c_str(), mtPath.c_str(), fsType, CONF_FSTAB);
        }
        else {
                cmd = s_printf("sed -i '$a %s %s %s %s 0 0' %s",
                               lvPath.c_str(), mtPath.c_str(), fsType, opt, CONF_FSTAB);
        }

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool RmToFstab(const std::string& lvPath)
{
        char* cmd = NULL;
        bool  bRet = false;

        cmd = s_printf("sed -i -e 's_^%s _#_' -e '/^#/d' %s", lvPath.c_str(), CONF_FSTAB);

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

// lvpath 的数据库记录 *代表获取所有 #代表free doing:1=used 0=unused -1=delete
int GetLvSql(std::vector<stLvInfo>& vl, const std::string lvpath)
{
        int ret = 0;
        MysqlData md;
        MysqlClient* mysql = NULL;
        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB error!\n");
                return ret;
        }
        // 
        string sql;
        if (lvpath.empty() || lvpath == LVPATH_ALL)
        {
                sql = "SELECT * FROM lvtype ORDER BY `name`";
        }
        else if (lvpath == LVPATH_FREE)
        {
                // 获取所有空闲的lv
                sql = "SELECT * FROM lvtype WHERE isused is NULL OR isused = 0 ORDER BY `name`";
        }
        else if (0 == lvpath.find(LVPATH_PREMOUNT))
        {
                // 获取所有mount为lvpath的lv
                sql = string("SELECT * FROM lvtype WHERE mount like '")+(lvpath.c_str()+1)+"' AND isused>0 ORDER BY `name`";
        }
        else
        {
                sql = "SELECT * FROM lvtype WHERE lvpath like '" + lvpath + "' OR name LIKE '"+lvpath+"' ORDER BY `name`";
        }
        if (!mysql->Query(sql, md))
        {
                delete mysql;
                LOG_ERR("Query Error! sql=%s\n", sql.c_str());
                return -1;
        }
        // 释放数据库资源
        delete mysql;

        uint row = md.Rows();
        //uint tarLvRow = 0xFFFFFFFF;
        stLvInfo newli;
        for (uint i=0; i<row; ++i)
        {
                MysqlDataRow mdr = md.GetRow(i);
                newli.path = mdr["lvpath"];
                newli.name = mdr["name"];
                newli.vgname = mdr["vgname"];
                newli.mount = mdr["mount"];
                newli.fsfmt = mdr["fstype"];
                newli.size = atoi(mdr["size"].c_str());
                newli.used = atoi(mdr["isUsed"].c_str());
                //newli.fmt();
                vl.push_back(newli);
        }
        // 如果找到 target 

        return ret;
}

// lvpath 的数据库记录 doing:见下
int SetLvSql(const stLvInfo& li, int doing)
{
        int ret = 0;
        //MysqlData md;
        MysqlClient* mysql = NULL;
        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB error!\n");
                return ret;
        }
        // 
        string sql = "SELECT * FROM lvtype";
        if (li.path.empty() || li.path == "*")
        {
                delete mysql;
                LOG_ERR("empty lvpath");
                return -1;
        }
        else
        {
        // doing:{1=used; 0=unused; -1=delete; 2=add; 3=set size; 4=set fs; 40=set fs=free;}
                if (LVSQL_UNUSED == doing)
                {
                        sql = "UPDATE lvtype SET isUsed='0' WHERE lvpath LIKE '" + li.path + "' OR name LIKE '"+li.name+"'";
                } else
                if (LVSQL_USED == doing)
                {
                        sql = "UPDATE lvtype SET isUsed='1' WHERE lvpath LIKE '" + li.path + "' OR name LIKE '"+li.name + "'";
                } else
                if (LVSQL_DEL == doing)
                {
                        sql = "DELETE FROM lvtype WHERE lvpath LIKE '" + li.path + "' OR name LIKE '"+li.name+"'";
                } else
                if (LVSQL_ADD == doing)
                {
                        sql = "INSERT INTO lvtype VALUES (NULL,'"+li.path+"','"+
                            li.name+"','"+li.vgname+"','"+li.mount+"','"+li.fsfmt+"','"+
                            IntToString(li.size)+"','"+IntToString(li.used)+"',NULL)";
                } else
                if (LVSQL_SETSIZE == doing)
                {
                        sql = "UPDATE lvtype SET size='"+IntToString(li.size)+"' WHERE lvpath LIKE '"+li.path+"' OR name LIKE '"+li.name+"'";
                } else
                if (LVSQL_SETFS == doing)
                {
                        sql = "UPDATE lvtype SET fstype='"+li.fsfmt+"', isUsed="+(char)((li.used%10)+'0')+", mount='"+li.mount+"' WHERE lvpath LIKE '"+li.path+"' OR name LIKE '"+li.name+"'";
                } else
                if (LVSQL_FREEFS == doing)
                {
                        sql = "UPDATE lvtype SET fstype='', mount='', isUsed=0 WHERE lvpath LIKE '"+li.path+"' OR name LIKE '"+li.name+"'";
                }
        }
        if (!mysql->Query(sql))
        {
                delete mysql;
                LOG_ERR("Query Error! sql=%s\n", sql.c_str());
                return -1;
        }
        // 释放数据库资源
        delete mysql;

        return 0;
}
}