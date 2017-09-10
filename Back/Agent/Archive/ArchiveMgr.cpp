#include "ArchiveMgr.h"

#include <map>
#include <stdio.h>
#include <Include/Types.h>
#include "Include/String.h"
#include "Public/Mysql/Mysql.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Crontab/Crontab.h"
#include "Utils/Common/CommonFunc.h"

using std::map;

ArchiveMgr::ArchiveMgr()
{
}

ArchiveMgr::~ArchiveMgr()
{
}

int ArchiveMgr::AddArchiveTask(struct _ArchiveConfig& archiveConfig)
{
        int ret = 0;
        // 获取最大值
        int maxId = 0;
        ret = GetMaxId(maxId);
        if (ret < 0) {
                LOG_ERR("GetMaxId Error!\n");
                return ret;
        }

        // 当前归档配置目录
        ConfigReader<struct _ArchiveConfig> crArchive(IntToString(maxId));
        ret = crArchive.Write(archiveConfig);
        if (ret < 0) {
                LOG_ERR("write error! path=%d\n", maxId);
        }
        return ret;
}

int ArchiveMgr::DelArchiveTask(const string &id)
{
        int ret = 0;
        MysqlClient* mysql = NULL;

        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB error!\n");
                return ret;
        }
        // 1. 删除数据库中归档信息
        string sql = "delete from archive where id=" + id;
        if (!mysql->Query(sql)) {
                delete mysql;
                LOG_ERR("Query Error! sql=%s\n", sql.c_str());
                return -1;
        }
        
        //删除路径对
        sql = "delete from archive_path_pair where config_id=" + id;
        if (!mysql->Query(sql)) {
                delete mysql;
                LOG_ERR("Query Error! sql=%s\n", sql.c_str());
                return -2;
        }

        //删除日志,删除还没有归档的，状态为scan的日志
        sql = "delete from archive_log where config_id=" + id + " and state='scan'";
        if (!mysql->Query(sql)) {
                delete mysql;
                LOG_ERR("Query Error! sql=%s\n", sql.c_str());
                return -3;
        }

        // 删除过滤信息
        sql = "delete from archive_filter where archive_id=" + id;
        if (!mysql->Query(sql)) {
                delete mysql;
                LOG_ERR("Query Error! sql=%s\n", sql.c_str());
                return -4;
        }

        // 删除归档各节点状态
        sql = "delete from node_archive_state where archive_id=" + id;
        if (!mysql->Query(sql)) {
                delete mysql;
                LOG_ERR("Query Error! sql=%s\n", sql.c_str());
                return -5;
        }

        // 释放数据库资源
        delete mysql;

        return ret;
}

int ArchiveMgr::ModifyArchiveTask(const string &id, 
                                const struct _ArchiveConfig& archiveConfig)
{
        int ret = 0;
        ConfigReader<struct _ArchiveConfig> crArchive(id);
        ret = crArchive.Write(archiveConfig);
        if (ret < 0) {
                LOG_ERR("write error! path=%s\n", id.c_str());
        }
        return ret;
}

int ArchiveMgr::LookupArchiveInfo(const string &id, 
                                struct _ArchiveConfig& archiveConfig)
{
        int ret = 0;
        //获取新加归档任务配置文件全路径
        ConfigReader<struct _ArchiveConfig> crArchive(id);
        ret = crArchive.Read(archiveConfig);
        if (ret < 0) {
                LOG_ERR("read error! path=%s\n", id.c_str());
        }
        return ret;
}

int ArchiveMgr::SetArchiveState(const string& id, const string& nodeId, const string& state)
{
        int ret = 0;
        MysqlData md;
        MysqlClient* mysql = NULL;

        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB Error!\n");
                return ret;
        }
        string sql = "update node_archive_state set archive_state='" + state + "' where archive_id=" +
                     id + " and data_node_id=" + nodeId;
        if (!mysql->Query(sql)) {
                LOG_ERR("mysql query error: \n", sql.c_str());
                ret = -1;
        }
        delete mysql;
        return ret;
}

int ArchiveMgr::GetArchiveState(const string& id, const string& nodeId, string& state)
{
        int ret = 0;
        MysqlData md;
        MysqlClient* mysql = NULL;

        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB Error!\n");
                return ret;
        }
        string sql = "select archive_state from node_archive_state where archive_id=" + 
                     id + " and data_node_id=" + nodeId;
        if (!mysql->Query(sql, md)) {
                delete mysql;
                LOG_ERR("mysql query error: \n", sql.c_str());
                return -1;
        }
        state = md.GetData(0, "archive_state");
        delete mysql;

        return 0;
}

int ArchiveMgr::GetMaxId(int& maxId)
{
        int ret = 0;
        MysqlData md;
        MysqlClient* mysql = NULL;

        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB Error!\n");
                return ret;
        }
        string sql = "select ifnull(max(id),0)+1 as id from archive";
        if (!mysql->Query(sql, md)) {
                delete mysql;
                LOG_ERR("mysql query error: \n", sql.c_str());
                return -2;
        }
        maxId = atoi(md.GetData(0,0).c_str());
        delete mysql;

        return 0;
}

// int ArchiveMgr::LookupArchiveTaskList(map<int, string>& archiveTaskList)
// {
//         int ret = 0;
//         MysqlData md;
//         MysqlClient* mysql = NULL;
// 
//         // 获取数据库连接
//         ret = GetDB(&mysql);
//         if (ret < 0) {
//                 LOG_ERR("GetDB error!\n");
//                 return ret;
//         }
// 
//         string sql = "select id, name from archive";
//         if (!mysql->Query(sql, md)) {
//                 delete mysql; 
//                 LOG_ERR("query sql error! sql=%s\n", sql.c_str());
//                 return -1;
//         }
// 
//         int row = md.Rows();
//         for (int i=0; i<row; ++i) {
//                 MysqlDataRow mdr = md.GetRow(i);
//                 int    id   = atoi(mdr["id"].c_str());
//                 string name = mdr["name"];
//                 archiveTaskList.insert(make_pair(id, name));
//         }
// 
//         delete mysql; 
//         return 0;
// }

// int ArchiveMgr::StartArchiveTask(int configId)
// {
//         /* ArchiveMgr.Stop() */
//         return 0;
// }
// 
// int ArchiveMgr::StopArchiveTask(int configId)
// {
//         /* ArchiveMgr.Start() */
//         return 0;
// }
// 
// /* 仅在服务器端的代理调用 */
// int ArchiveMgr::StartArchiveService(int configId)
// {
//         return 0;
// }
// 
// /* 仅在服务器端的代理调用 */
// int ArchiveMgr::StopArchiveService(int configId)
// {
//         return 0;
// }

