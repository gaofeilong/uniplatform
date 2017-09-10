/*******************************************************************************
 *
 * @ File Name  : NfsMgr.cpp
 * @ Date       : 2013-10-21
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: NFS 模块功能处理
 * @ History    : 2013-10-21：创建
 *
 * ****************************************************************************/

#include <map>
#include <vector>
#include <string>
#include <stdio.h>

#include "NfsMgr.h"

#include "Include/String.h"
#include "Agent/Fs/DdfsMgr.h"
#include "Agent/Fs/AdfsMgr.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Common/CommonFunc.h"
#include "Public/Mysql/Mysql.h"

using std::map;
using std::vector;
using std::string;
using std::make_pair;

#define make_str_pair   make_pair<string, string>

NfsMgr::NfsMgr()
{ 
}

NfsMgr::~NfsMgr() 
{
}

int NfsMgr::GetInfo(vector<struct NfsServerInfo> &nfsInfoList)
{
        struct NfsServerInfo tmpNfsServerInfo;
        struct NfsClientInfo tmpNfsClientInfo;

        // get nfs config from db;
        MysqlData md;
        MysqlDataRow mdr;
        MysqlClient* mysql;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }
        if (!mysql->Query(SQL_SELECT_NFS_INFO, md)) {
                LOG_ERR("mysql query error: sql=%s\n", SQL_SELECT_NFS_INFO);
                delete mysql;
                return -2;
        }

        // get nfs service state
        string type;
        int tmpid = -1;
        vector<string> err;
        string state = Execute(CL_NFS_STATUS, err) == 0? "running": "stopped";

        for (size_t i = 0; i < md.Rows(); i++) {
                FsMgr *fsMgr;
                mdr = md.GetRow(i);

                if (GetMpType(mdr["path"], type) < 0) {
                        LOG_ERR("get mp: %s type error", mdr["path"].c_str());
                        delete mysql;
                        return -1;
                } else {
                        if (type == "adfs") {
                                fsMgr = new AdfsMgr;
                        } else {
                                fsMgr = new DdfsMgr;
                        }
                }

                // get online client list
                vector<string> conn;
                if (Execute(CL_NFS_ACTIVE_CLIENT(mdr["ip"], 
                        "root", mdr["password"], mdr["mp"]), err) < 0) {
                        LOG_WRN("active nfs client mount point info error");
                }
                if (Execute(CL_NFS_ONLINE_CLIENT, conn) < 0) {
                        LOG_WRN("get actived nfs client error");
                }

                tmpNfsClientInfo.ip       = mdr["ip"];
                tmpNfsClientInfo.password = mdr["password"];
                tmpNfsClientInfo.state    = 
                        find(conn.begin(), conn.end(), mdr["ip"])
                        == conn.end()?  "umounted": "mounted";
                if (tmpid != atoi(mdr["id"].c_str())) {
                        tmpNfsServerInfo.state     = state;
                        tmpNfsServerInfo.mp        = mdr["mp"];
                        tmpNfsServerInfo.name      = mdr["name"];
                        tmpNfsServerInfo.path      = mdr["path"];
                        tmpNfsServerInfo.parameter = mdr["parameter"];
                        tmpNfsServerInfo.mpState   = fsMgr->GetMpStatus(mdr["path"]);
                        nfsInfoList.push_back(tmpNfsServerInfo);
                }
                if (!tmpNfsClientInfo.ip.empty()) {
                        nfsInfoList.back().clientList.push_back(tmpNfsClientInfo);
                }
                tmpid = atoi(mdr["id"].c_str());
                delete fsMgr;
        }
        delete mysql;
        return 0;
}

int NfsMgr::Create(const struct NfsServerInfo &nfsServerInfo, 
                   vector<string> &errIp)
{
        FsMgr *fsMgr;
        string cmd, sql;
        vector<string> err;
        map<string, string> okIp;

        // 连接测试各个client, 筛选出OK的节点和ERR的节点
        // OK的节点用于添加到nfs, ERR的节点用于返回错误
        errIp.clear();
        vector<struct NfsClientInfo>::const_iterator vcIt = 
                        nfsServerInfo.clientList.begin();
        for (; vcIt != nfsServerInfo.clientList.end(); ++vcIt) {
                cmd = CL_FBS_CONNECT_TEST(vcIt->ip, "root", vcIt->password);
                if (Execute(cmd, err) != 0) {
                        errIp.push_back(vcIt->ip);
                } else {
                        okIp.insert(make_str_pair(vcIt->ip, vcIt->password));
                }
        }

        string type;
        if (GetMpType(nfsServerInfo.path, type) < 0) {
                LOG_ERR("get mp: %s type error", nfsServerInfo.path.c_str());
                return -1;
        } else {
                if (type == "adfs") {
                        fsMgr = new AdfsMgr;
                } else {
                        fsMgr = new DdfsMgr;
                }
        }

        if (fsMgr->GetMpStatus(nfsServerInfo.path) != "mounted") {
                LOG_ERR("shared path: %s not mounted", nfsServerInfo.path.c_str());
                return -6;
        }
        delete fsMgr;

        // 写入NFS配置文件并重载配置文件
        string item = nfsServerInfo.path + " *(" + nfsServerInfo.parameter + ")";
        if (AddNfsConfItem(item) < 0) {
                LOG_ERR("add nfs conf item %serror", item.c_str());
                return -3;
        }
        cmd = CL_NFS_RELOAD_CONF;
        if (Execute(cmd, err) < 0) {
                LOG_ERR("execute [%s] error: %s", cmd.c_str(), err.back().c_str());
                if (RemoveNfsConfItem(item) < 0) {
                        LOG_ERR("remove nfs conf item %s error", item.c_str());
                }
                return -5;
        }

        MysqlData md;
        MysqlClient* mysql;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }
        
        // 插入数据库表 nfsserver
        sql = SQL_INSERT_NFS_SERVER(nfsServerInfo.name, nfsServerInfo.path,
                                    nfsServerInfo.mp, nfsServerInfo.parameter);
        if (!mysql->Query(sql)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                delete mysql;
                return -2;
        }

        // 查询数据库表 nfsserver 中刚才新增记录的ID
        sql = SQL_SELECT_NFS(nfsServerInfo.name);
        if (!mysql->Query(sql, md)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                delete mysql;
                return -3;
        }
        string id = md.GetData(0, "id");

        // 插入数据库表 nfsclient
        map<string, string>::iterator mssIt = okIp.begin();
        for (; mssIt != okIp.end(); ++mssIt) {
                sql = SQL_INSERT_NFS_CLIENT(id, mssIt->first, mssIt->second);
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        delete mysql;
                        return -4;
                }
        }
        delete mysql;
        return 0;
}

// 这里nfsServerInfo.clientList中的ip都是新增的ip
int NfsMgr::Edit(const struct NfsServerInfo &nfsServerInfo, vector<string> &errIp)
{
        // 连接测试各个新增的client
        // 筛选出OK的节点和ERR的节点，OK的节点用于添加到nfs，
        //      ERR的节点用于返回错误，如果ERR的节点已经存在于
        //      NFS表中，并且密码相同，则从表中删除该节点
        // 写入NFS配置文件、插入数据库表 nfsserver 和 nfsclient
        // 将失败的节点返回
        string cmd, sql;
        vector<string> err;
        map<string, string> okIp;

        // 连接测试各个client, 筛选出OK的节点和ERR的节点
        // OK的节点用于添加到nfs, ERR的节点用于返回错误
        errIp.clear();
        vector<struct NfsClientInfo>::const_iterator vcIt = 
                        nfsServerInfo.clientList.begin();
        for (; vcIt != nfsServerInfo.clientList.end(); ++vcIt) {
                cmd = CL_FBS_CONNECT_TEST(vcIt->ip, "root", vcIt->password);
                if (Execute(cmd, err) != 0) {
                        errIp.push_back(vcIt->ip);
                } else {
                        okIp.insert(make_str_pair(vcIt->ip, vcIt->password));
                }
        }

        MysqlData md;
        MysqlClient* mysql;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }
        
        // 查询数据库表 nfsserver 中的记录
        sql = SQL_SELECT_NFS(nfsServerInfo.name);
        if (!mysql->Query(sql, md)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                delete mysql;
                return -3;
        }
        string id = md.GetData(0, "id");
        string oldParameter = md.GetData(0, "parameter");

        if (oldParameter != nfsServerInfo.parameter) {
                string oldItem = md.GetData(0, "path") + " *(" + oldParameter + ")";
                string newItem = nfsServerInfo.path + " *(" + 
                                 nfsServerInfo.parameter + ")";
                if (ReplaceNfsConfItem(oldItem, newItem) < 0) {
                        LOG_ERR("edit nfs conf item %s to %s error", 
                                        oldItem.c_str(), newItem.c_str());
                        delete mysql;
                        return -4;
                }

                // 重载配置文件
                cmd = CL_NFS_RELOAD_CONF;
                if (Execute(cmd, err) < 0) {
                        LOG_ERR("execute [%s] error: %s", 
                                        cmd.c_str(), err.back().c_str());
                        if (ReplaceNfsConfItem(newItem, oldItem) < 0) {
                                LOG_ERR("recover nfs item %s to %s error", 
                                         newItem.c_str(), oldItem.c_str());
                        }
                        if (Execute(cmd, err) < 0) {
                                LOG_ERR("execute [%s] error: %s", 
                                        cmd.c_str(), err.back().c_str());
                        }
                        delete mysql;
                        return -3;
                }

                // 更新数据库表
                sql = SQL_UPDATE_NFS_SERVER(id, nfsServerInfo.parameter);
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        ReplaceNfsConfItem(newItem, oldItem);
                        delete mysql;
                        return -5;
                }
        }

        // 插入数据库表 nfsclient
        map<string, string>::iterator mssIt = okIp.begin();
        for (; mssIt != okIp.end(); ++mssIt) {
                sql = SQL_INSERT_NFS_CLIENT(id, mssIt->first, mssIt->second);
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        delete mysql;
                        return -4;
                }
        }

        delete mysql;
        return 0;
}

int NfsMgr::Remove(const string &name)
{
        // 只能删除stopped的NFS
        // 查出要删除nfs任务的id
        // 从nfsserver和nfsclient表中删除id和serverid等于该值的记录
        // 查询数据库表 nfsserver 中刚才新增记录的ID

        // 查出数据库中要删除的nfs的记录，得到nfs配置行和id
        string sql;
        MysqlData md;
        MysqlClient* mysql;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }

        sql = SQL_SELECT_NFS(name);
        if (!mysql->Query(sql, md)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                delete mysql;
                return -2;
        }
        string id = md.GetData(0, "id");
        string item = md.GetData(0, "path") + " *(" + 
                      md.GetData(0, "parameter") + ")";

        // 清除配置文件中的行
        if (RemoveNfsConfItem(item) < 0) {
                LOG_ERR("remove nfs conf item %s error", item.c_str());
                delete mysql;
                return -3;
        }
        // 清除数据库中该id的nfs记录
        sql = SQL_DELETE_NFS_CLIENT_BY_ID(id);
        if (!mysql->Query(sql)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
        }
        sql = SQL_DELETE_NFS_SERVER_BY_ID(id);
        if (!mysql->Query(sql)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
        }
        delete mysql;

        // 重载配置文件
        vector<string> err;
        string cmd = CL_NFS_RELOAD_CONF;
        if (Execute(cmd, err) < 0) {
                LOG_ERR("execute [%s] error: %s", cmd.c_str(), err.back().c_str());
                return -3;
        }

        return 0;
}


int NfsMgr::Start()
{
        vector<string> err;
        if (Execute(CL_NFS_START, err) != 0) {
                LOG_ERR("execute [%s] error: %s", 
                        CL_NFS_START, err.back().c_str());
                return -1;
        }
        return 0;
}

int NfsMgr::Stop()
{
        vector<string> err;
        if (Execute(CL_NFS_STOP, err) != 0) {
                LOG_ERR("execute [%s] error: %s", 
                        CL_NFS_STOP, err.back().c_str());
                return -1;
        }
        return 0;
}

int NfsMgr::MountClient(const string &name, const string &ip)
{
        // 查出该节点所在nfs的serverip、path和节点的mp、password
        // 执行远程命令，挂载nfs
        string sql;
        MysqlData md;
        MysqlClient* mysql;
        string path, mp, password;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }

        sql = SQL_SELECT_NFS_INFO;
        if (!mysql->Query(sql, md)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                delete mysql;
                return -2;
        }
        for (size_t i = 0; i < md.Rows(); i++) {
                if (name == md.GetData(i, "name") && ip == md.GetData(i, "ip")) {
                        mp       = md.GetData(i, "mp");
                        path     = md.GetData(i, "path");
                        password = md.GetData(i, "password");
                        break;
                }
                if (i == md.Rows() - 1) {
                        LOG_ERR("cannot find nfs task :%s client ip: %s",
                                                name.c_str(), ip.c_str());
                        delete mysql;
                        return -3;
                }
        }
        delete mysql;

        // 获取NFS服务器IP地址
        vector<string> err;
        string cmd = CL_GET_SERVER_IP;
        if (Execute(cmd, err) < 0) {
                LOG_ERR("execute [%s] error: %s", cmd.c_str(), err.back().c_str());
                return -3;
        }

        // 远程命令
        cmd = CL_NFS_REMOTE_MOUNT(ip, "root", password, err.back(), path, mp);
        if (Execute(cmd, err) < 0) {
                LOG_ERR("execute [%s] error: %s", cmd.c_str(), err.back().c_str());
                return -4;
        }
        return 0;
}

int NfsMgr::UmountClient(const string &name, const string &ip)
{
        // 查出nfs的id
        // 查出该该id下该节点的mp、password
        // 执行远程命令，卸载nfs
        string sql;
        MysqlData md;
        MysqlClient* mysql;
        string path, mp, password;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }

        sql = SQL_SELECT_NFS_INFO;
        if (!mysql->Query(sql, md)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                delete mysql;
                return -2;
        }
        for (size_t i = 0; i < md.Rows(); i++) {
                if (name == md.GetData(i, "name") && ip == md.GetData(i, "ip")) {
                        mp       = md.GetData(i, "mp");
                        password = md.GetData(i, "password");
                        break;
                }
        }
        delete mysql;

        // 远程命令
        vector<string> err;
        string cmd = CL_NFS_REMOTE_UMOUNT(ip, "root", password, mp);
        if (Execute(cmd, err) < 0) {
                LOG_ERR("execute [%s] error: %s", cmd.c_str(), err.back().c_str());
                return -3;
        }
        return 0;
        return 0;
}

int NfsMgr::RemoveClient(const string &name, const string &ip)
{
        // 只有卸载的节点能被删除
        // 查出该NFS任务名称所在nfs的id
        // 删除nfsclient表中serverid等于该id并且ip等于该ip的记录
        string sql;
        MysqlData md;
        MysqlClient* mysql;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }

        sql = SQL_SELECT_NFS(name);
        if (!mysql->Query(sql, md)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                delete mysql;
                return -2;
        }
        string id = md.GetData(0, "id");

        sql = SQL_DELETE_NFS_CLIENT_BY_ID_IP(id, ip);
        if (!mysql->Query(sql)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                delete mysql;
                return -3;
        }
        delete mysql;
        return 0;
}

int NfsMgr::RemoveNfsConfItem(const string &item)
{
        FILE *stream = fopen(NFS_CONF_PATH, "r+");
        if (NULL == stream) {
                LOG_ERR("open file %s error", NFS_CONF_PATH);
                return -1;	
        }

        string line;
        size_t lineLen = 0;
        char*  lineBuf = NULL;
        vector<string> Contents;
        while (getline(&lineBuf, &lineLen, stream) != -1) {
                if ((line = lineBuf) == item + "\n") {
                        continue;
                }
                Contents.push_back(line);        
        }

        free(lineBuf);
        rewind(stream);
        ftruncate(fileno(stream), 0); 
        vector<string>::iterator it = Contents.begin();
        for ( ; it != Contents.end(); ++it) {
                fprintf(stream, "%s", it->c_str());
        }
        fclose(stream);

        return 0;
}

int NfsMgr::AddNfsConfItem(const string &item)
{
        FILE *stream = fopen(NFS_CONF_PATH, "a");
        if (NULL == stream) {
                LOG_ERR("open file %s error", NFS_CONF_PATH);
                return -1;
        }
        fprintf(stream, "%s\n", item.c_str());
        fclose(stream);

        return 0;
}

int NfsMgr::ReplaceNfsConfItem(const string oldItem, const string &newItem)
{
        if (RemoveNfsConfItem(oldItem) < 0) {
                LOG_ERR("remove old nfs conf item %s error", oldItem.c_str());
                return -1;
        }
        if (AddNfsConfItem(newItem) < 0) {
                LOG_ERR("add new nfs conf item %s error", newItem.c_str());
                return -2;
        }
        return 0;
}

int NfsMgr::GetMpType(const string &mp, string &type)
{
        MysqlData md;
        MysqlClient* mysql;

        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }

        string sql = SQL_SELECT_DDFS_MP(mp);
        if (!mysql->Query(sql, md)) {
                LOG_ERR("mysql query error: sql=%s", sql.c_str());
                delete mysql;
                return -2;
        } else {
                if (md.Rows() > 0) {
                        type = "ddfs";
                        delete mysql;
                        return 0;
                }
        }

        sql = SQL_SELECT_ADFS_MP(mp.substr(0, mp.size() - 3));
        if (!mysql->Query(sql, md)) {
                LOG_ERR("mysql query error: sql=%s", sql.c_str());
                delete mysql;
                return -3;
        } else {
                if (md.Rows() > 0) {
                        type = "adfs";
                        delete mysql;
                        return 0;
                }
        }

        LOG_ERR("cannot find mp: %s in neither ddfsmp nor adfsmp", mp.c_str());
        delete mysql;
        return -1;
}
