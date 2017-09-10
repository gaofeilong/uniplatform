#include "ServiceAccess.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include "Public/Mysql/Mysql.h"

/*
#include "Smb.h"
#include "Nfs.h"
#include "Rsync.h"
#include "Ftp.h"
#include "Webdav.h"
*/
// 删除冗余配置，此函数依次删除rsync,ftp,webdav中的冗余配置。参数：用户名，@用户组名
int CServiceAccess::RemoveUser(const std::vector<std::string>& vu, int nReload)
{
/*
        CSmb::RemoveUser(vu);
        CRsync::RemoveUser(vu);
        CFtp::RemoveUser(vu);
        CWebdav::RemoveUser(vu);
        if (nReload)
        {
                LoadAll();
        }
*/
        return 0;
}

// 删除冗余ip，参数格式 ip/masklen
int CServiceAccess::RemoveIp  (const std::vector<std::string>& vip, int nReload)
{
/*
        CSmb::RemoveIp(vip);
        CNfs nfs;
        nfs.RemoveIp(vip);
        CRsync::RemoveIp(vip);
        CFtp::RemoveIp(vip);
        CWebdav::RemoveIp(vip);
        if (nReload)
        {
                LoadAll();
        }
*/
        return 0;
}

int CServiceAccess::RemoveGroup(const std::vector<std::string>& vg, int nReload)
{
/*
        std::vector<std::string> vu;
        unsigned int i = 0;
        for (i=0; i<vg.size(); ++i)
        {
                vu.push_back(std::string("@") + vg[i]);
        }
        CSmb::RemoveUser(vu);
        //CRsync::RemoveUser(vu);
        CFtp::RemoveUser(vu);
        CWebdav::RemoveUser(vu);
        if (nReload)
        {
                LoadAll();
        }
*/
        return 0;
}

void CServiceAccess::LoadAll()
{
/*
        CSmb smb;
        smb.LoadIn();
        CNfs nfs;
        nfs.LoadIn();
        CRsync rsync;
        rsync.LoadIn();
        CFtp ftp;
        ftp.LoadIn();
        CWebdav web;
        web.LoadIn();
*/
}

int CServiceAccess::ListShare()
{
        return RET_SUCCESS;
}

int CServiceAccess::AddShare()
{
        return RET_SUCCESS;
}

int CServiceAccess::DelShare()
{
        return RET_SUCCESS;
}

int CServiceAccess::GetShare()
{
        return RET_SUCCESS;
}

int CServiceAccess::SetShare()
{
        return RET_SUCCESS;
}

int CServiceAccess::GetShSql(std::vector<stShare>&vd, std::string name)
{
        int ret = 0;
        MysqlData md;
        MysqlClient* mysql = NULL;
        ret = GetDB(&mysql);
        if (ret < 0)
        {
                LOG_ERR("GetDB error!\n");
                return ret;
        }
        // 
        string sql;
        if (name.empty() || name == SHNAME_ALL)
        {
                sql = "SELECT * FROM share ORDER BY `name`";
        }
        else if (name == SHNAME_FREE)
        {
                // 获取所有空闲的 name
                sql = "SELECT * FROM share WHERE mask IS NULL OR mask= 0 ORDER BY `name`";
        }
        else
        {
                sql = "SELECT * FROM share WHERE name LIKE '"+name+"' ORDER BY `name`";
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
        stShare newli;
        for (uint i=0; i<row; ++i)
        {
                MysqlDataRow mdr = md.GetRow(i);
                newli.name = mdr["name"];
                newli.lv = mdr["lv"];
                newli.mask = atoi(mdr["mask"].c_str());
                //newli.fmt();
                vd.push_back(newli);
        }
        // 如果找到 target 

        return ret;
}

int CServiceAccess::SetShSql(const stShare& sh, int doing)
{
        int ret = 0;
        // 
        string sql = "SELECT * FROM share";
        if (sh.name.empty())
        {
                LOG_ERR("SetShSql(): empty share name");
                return -1;
        }
        else
        {
        // doing:{-1=del; 2=add; 3=set mask;}
                if (SHSQL_DEL == doing)
                {
                        sql = "DELETE FROM share WHERE name LIKE '"+sh.name+"'";
                } else
                if (SHSQL_ADD == doing)
                {
                        sql = "INSERT INTO share VALUES (NULL,'"+sh.name+"','"+
                            sh.lv+"','"+IntToString(sh.mask)+"',NULL)";
                } else
                if (SHSQL_SETMASK == doing)
                {
                        sql = "UPDATE share SET mask='"+IntToString(sh.mask)+"' WHERE name LIKE '"+sh.name+"'";
                }
        }
        //MysqlData md;
        MysqlClient* mysql = NULL;
        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB error!\n");
                return ret;
        }
        if (!mysql->Query(sql))
        {
                delete mysql;
                LOG_ERR("Query Error! sql=%s\n", sql.c_str());
                return -1;
        }
        // 释放数据库资源
        delete mysql;
        return RET_SUCCESS;
}
