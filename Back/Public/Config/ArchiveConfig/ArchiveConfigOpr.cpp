/* 
 * @ Date        : 2013-04-16
 * @ Auther      : wangbo
 * @ Description : 对Archive配置文件进行操作(读,写)
 * @ Email       : wang.bo@scistor.com
 *
 * @ History     : 2013-04-16 创建
 *
 */

#include "ArchiveConfigOpr.h"

#include <stdio.h>
#include <string.h>

#include "Include/Types.h"
#include "Include/String.h"
#include "Public/Mysql/Mysql.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Config/IniConfig.h"
#include "Utils/Common/DirFileOpr.h"
#include "Utils/Common/CommonFunc.h"

ArchiveConfigOpr::ArchiveConfigOpr(const string& configPath):
                       ConfigOpr<struct _ArchiveConfig>(configPath)
{
}

ArchiveConfigOpr::~ArchiveConfigOpr()
{
}

int ArchiveConfigOpr::Read(struct _ArchiveConfig &archiveConfig)
{
        int ret = 0;

        // 连接数据库
        MysqlClient* mysql = NULL;
        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB error!\n");          
                return ret;
        }
        // 获取归档ID
        string id = m_ConfigPath;

        MysqlData md;
        MysqlDataRow mdr;
        // 获取归档配置信息
        string sql = "select * from archive,archive_filter where archive.id=" + id + 
                     " and archive_filter.archive_id=" + id;
        //string sql = "select * from archive where archive.id=" + id;
        if (!mysql->Query(sql, md)) {
                delete mysql;
                LOG_ERR("mysql query errro! sql=%s\n", sql.c_str());
                return -1;
        }
        // 只有一行符合要求
        mdr = md.GetRow(0);
        archiveConfig.id             = atoi(mdr["id"].c_str());
        archiveConfig.name           = mdr["name"];
        archiveConfig.startTime      = mdr["start_time"];
        archiveConfig.duration       = atoi(mdr["duration"].c_str());
        archiveConfig.interval       = atoi(mdr["start_interval"].c_str());
        archiveConfig.modifyTime     = atoi(mdr["modify_time"].c_str());
        archiveConfig.isCheck        = atoi(mdr["is_check"].c_str());
        archiveConfig.isDeleteSource = atoi(mdr["is_delete_source"].c_str());
        // 过滤信息
        archiveConfig.prefixType     = atoi(mdr["prefix_type"].c_str());
        archiveConfig.prefix         = mdr["prefix"];
        archiveConfig.postfixType    = atoi(mdr["postfix_type"].c_str());
        archiveConfig.postfix        = mdr["postfix"];
        archiveConfig.dirType        = atoi(mdr["dir_type"].c_str());
        archiveConfig.dir            = mdr["dir"];
        archiveConfig.sizeType       = atoi(mdr["size_type"].c_str());
        archiveConfig.minSize        = atol(mdr["min_size"].c_str());
        archiveConfig.maxSize        = atol(mdr["max_size"].c_str());

        // 获取归档对应IP列表
        // 1. 通过ID获取IP
        sql = "select * from node_archive_state where archive_id="+id;
        if (!mysql->Query(sql, md)) {
                delete mysql;
                LOG_ERR("mysql query errro! sql=%s\n", sql.c_str());
                return -1;
        }
        for (size_t i = 0; i < md.Rows(); ++i) {
                mdr = md.GetRow(i);
                string ip;
                int    counter  = atoi(mdr["counter"].c_str());
                string arvState = mdr["archive_state"];
                string isError  = mdr["is_error"];
                string nodeId   = mdr["data_node_id"];
                if (nodeId == "-1") {
                        ip = "127.0.0.1";
                } else {
                        ret = GetIpById(nodeId, ip);
                        if (ret < 0) {
                                LOG_ERR("GetIpById Error!\n");
                                return ret;
                        }
                }
                archiveConfig.arvStateSet.insert(make_pair(ip,ArchiveState(ip, arvState, counter, isError)));
        }

        // 获取归档路径对
        sql = string("select src_path, dest_path, backup_path from ") +
                     "archive_path_pair where config_id=" + id;
        if (!mysql->Query(sql, md)) {
                delete mysql;
                LOG_ERR("mysql query errro! sql=%s\n", sql.c_str());
                return -2;
        }

        for (size_t i = 0; i < md.Rows(); ++i) {
                mdr = md.GetRow(i);
                archiveConfig.pathPair.push_back(PathPair(mdr["src_path"], 
                                        mdr["dest_path"], mdr["backup_path"]));
        }
        delete mysql;

        return ret;
}

int ArchiveConfigOpr::Write(const struct _ArchiveConfig &archiveConfig)
{
        int ret = 0;

        // 数据库连接
        MysqlClient* mysql = NULL;
        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB error!\n");
                return ret;
        }
        string id = m_ConfigPath;

        // 插入archive，存在时删除重新插入
        ret = Exist2Delete(mysql, "archive", "id", id);
        if (ret < 0) {
                delete mysql;
                LOG_ERR("Exist2Delete Error!\n");
                return ret;
        }
        // 重新插入
        string sql = string("insert into archive(id,name,start_time,duration,") +
                            "start_interval,modify_time,is_check,is_delete_source) values(";

        sql +=  id + ",'" + archiveConfig.name + "','" + archiveConfig.startTime + 
                "'," + IntToString(archiveConfig.duration) + "," + 
                IntToString(archiveConfig.interval) + "," + IntToString(archiveConfig.modifyTime) +
                "," + IntToString(archiveConfig.isCheck) +
                "," + IntToString(archiveConfig.isDeleteSource) + ")";

        if (!mysql->Query(sql)) {
                delete mysql;
                LOG_ERR("mysql query error! sql=%s\n", sql.c_str());
                return -1;
        }

        // 插入过滤条件信息
        ret = Exist2Delete(mysql, "archive_filter", "archive_id", id);
        if (ret < 0) {
                delete mysql;
                LOG_ERR("Exist2Delete Error!\n");
                return ret;
        }
        sql = string("insert into archive_filter(archive_id,prefix_type,prefix,postfix_type,postfix," ) +
                     "dir_type,dir,size_type,min_size,max_size) values(";

        sql += id + "," + IntToString(archiveConfig.prefixType) + ",'"  + archiveConfig.prefix + 
               "'," + IntToString(archiveConfig.postfixType) + ",'" + archiveConfig.postfix + "'," + 
               IntToString(archiveConfig.dirType) + ",'" + archiveConfig.dir + "'," +
               IntToString(archiveConfig.sizeType) + "," + IntToString(archiveConfig.minSize) + 
               "," + IntToString(archiveConfig.maxSize) + ")";

        if (!mysql->Query(sql)) {
                delete mysql;
                LOG_ERR("mysql query error! sql=%s\n", sql.c_str());
                return -2;
        }

        // 插入路径路径对
        ret = Exist2Delete(mysql, "archive_path_pair", "config_id", id);
        if (ret < 0) {
                delete mysql;
                LOG_ERR("Exist2Delete Error!\n");
                return ret;
        }
        // 组装SQL语句
        vector<struct PathPair>::const_iterator vpIt = 
                                        archiveConfig.pathPair.begin();
        for (; vpIt != archiveConfig.pathPair.end(); ++vpIt) {
                sql = string("insert into archive_path_pair(`config_id`, ") + 
                             "`src_path`, `dest_path`, `backup_path`) values(" +
                             id + ",'" + vpIt->m_Src + "','" + vpIt->m_Dest +
                             "','" + vpIt->m_Backup + "')";
                if (!mysql->Query(sql)) {
                        delete mysql;
                        LOG_ERR("mysql query error! sql=%s\n", sql.c_str());
                        return -3;
                }
        }

        // 插入归档状态 
        ret = Exist2Delete(mysql, "node_archive_state", "archive_id", id);
        if (ret < 0) {
                delete mysql;
                LOG_ERR("Exist2Delete Error!\n");
                return ret;
        }
        map<string,ArchiveState>::const_iterator mssIter = archiveConfig.arvStateSet.begin();
        for (; mssIter != archiveConfig.arvStateSet.end(); ++mssIter) {
                // 归档状态,初始为 stop
                string nodeId;
                if (mssIter->first == LOOP_BACK_IP) {
                        nodeId = "-1";
                } else {
                        ret = GetIdByIp(mssIter->first, nodeId);
                        if (ret < 0) {
                                LOG_ERR("get node %s id error", mssIter->first.c_str());
                                continue;
                        } 
                }
                sql = "insert into node_archive_state values(" + id + "," + nodeId + ",0,'" + 
                                                             mssIter->second.archiveState + "',0)";
                if (!mysql->Query(sql)) {
                        ret = -1;
                        LOG_ERR("mysql query error! sql=%s\n", sql.c_str());
                        break;
                }
        }

        delete mysql;
        return ret;
}

int ArchiveConfigOpr::Exist2Delete(MysqlClient* mysql, const string& tableName, 
                                  const string& idName, const string& idVal)
{
        int ret = 0;
        string sql = "delete from " + tableName + " where " + idName + "=" + idVal;
        if (!mysql->Query(sql)) {
                LOG_ERR("mysql query error! sql=%s\n", sql.c_str());
                ret = -1;
        }
        return ret;
}
