#include "InstallMgr.h"
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Include/String.h"
#include "Server/Install/Protocol.h"
#include "Utils/Lightev/Socket.h"
#include "Utils/Lightev/EVCommon.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Config/IniConfig.h"
#include "Utils/Common/CommonFunc.h"
#include "Public/Config/ConfigReader.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>

using boost::bind;
using boost::thread;

InstallMgr::InstallMgr()
{
}

InstallMgr::~InstallMgr()
{
}

int InstallMgr::ConnectIndexNode(const string &user, const string &password, const string &ip)
{
        vector<string> ipList;
        ipList.push_back(ip);
        thread t1(bind(&InstallMgr::StartConnect, this, user, password, ipList, "index", "0"));
        return 0;
}

int InstallMgr::ConnectDataNode(const string &user, const string &password, const vector<string> &ipList, const string &indexId)
{
        thread t1(bind(&InstallMgr::StartConnect, this, user, password, ipList, "data", indexId));
        return 0;
}

int InstallMgr::StartConnect(const string &user, const string &password, 
        const vector<string> &ipList, const string &type, const string &indexId)
{
        int ret = 0;
        string sql;
        vector<string> err;
        MysqlClient* mysql = NULL;
        vector<string>::const_iterator vsIt;

        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }

        // 将所有需要做连接测试的节点插入到临时表中
        if (!mysql->Query(SQL_TRUNCATE_FBS_CONENCTION_TEST)) {
                LOG_ERR("mysql query error: sql=%s\n", 
                                SQL_TRUNCATE_FBS_CONENCTION_TEST.c_str());
                delete mysql;
                return -2;
        }
        for (vsIt = ipList.begin(); vsIt != ipList.end(); ++vsIt) {
                sql = SQL_INSERT_FBS_CONNECTION_TEST(*vsIt, user, password, type, indexId);
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        delete mysql;
                        return -3;
                }
        }

        // 测试是否能够连接上，并将测试结果更新到表中状态字段
        for (vsIt = ipList.begin(); vsIt != ipList.end(); ++vsIt) {
                sql = SQL_UPDATE_FBS_CONNECTION_TEST(*vsIt, "testing");
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        continue;
                }
                sql = Execute(CL_FBS_CONNECT_TEST(*vsIt, user, password), err) == 0? 
                      SQL_UPDATE_FBS_CONNECTION_TEST(*vsIt, "ok"):
                      SQL_UPDATE_FBS_CONNECTION_TEST(*vsIt, err[err.size() - 1]);
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        continue;
                }
        }

        delete mysql;
        return 0;
}

int InstallMgr::InstallIndexNode(const string &user, const string &password, const string &ip)
{
        vector<string> ipList;
        ipList.push_back(ip);
        thread t1(bind(&InstallMgr::StartInstallIndex, this, user, password, ip));
        return 0;
}

int InstallMgr::InstallStep(MysqlClient **mysql, const string &ip, const string &cmd, const string &process, const string &desc)
{
        vector<string> err;
        string sql = SQL_UPDATE_FBS_INSTALL(ip, process, desc, "waiting");
        if (!(*mysql)->Query(sql)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                return false;
        }
        if (Execute(cmd, err) < 0) {
                LOG_ERR("execute %s error %s", cmd.c_str(), err.back().c_str());
                sql = SQL_UPDATE_FBS_INSTALL(ip, process, desc, "error");
                if (!(*mysql)->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                }
                return false;
        }
        sql = SQL_UPDATE_FBS_INSTALL(ip, process, desc, "ok");
        if (!(*mysql)->Query(sql)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                return false;
        }
        return true;
}

int InstallMgr::InstallGatewayAgent(MysqlClient **mysql, const string &user, const string password, const string &ip, const string &ms)
{
        // 创建安装目录、拷贝安装包、安装网关代理端、启动网关代理
        if (!InstallStep(mysql, ip, CL_FBS_CREATE_INSTALL_DIR(ip, user, password), "0", "initialize env...") ||
            !InstallStep(mysql, ip, CL_FBS_COPY_INSTALL_PACKET(ip, user, password), "10", "copy install packet...") ||
            !InstallStep(mysql, ip, CL_FBS_INSTALL_GWAGENT(ip, user, password), "50", "install gwagent...") ||
            !InstallStep(mysql, ip, CL_FBS_SET_MYSQL_SERVER(ip, user, password, ms), "60", "update conf files...") ||
            !InstallStep(mysql, ip, CL_FBS_STARTUP_GWAGENT(ip, user, password), "70", "startup gwagent...")) {
                return -1;
        }
        return 0;
}

int InstallMgr::StartInstallIndex(const string &user, const string &password, const string ip)
{
        // 安装、启动网关代理
        MysqlData md;
        MysqlClient* mysql;
        string sql, serverIp;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }

        if (!mysql->Query(SQL_TRUNCATE_FBS_INSTALL)) {
                LOG_ERR("mysql query error: sql=%s\n", SQL_TRUNCATE_FBS_INSTALL.c_str());
                goto outInstallIndex;
        }
        sql = SQL_INSERT_FBS_INSTALL(ip, user, password, "index", "-1");
        if (!mysql->Query(sql)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                goto outInstallIndex;
        }
        if (GetServerPortIp(serverIp) < 0) {
                LOG_ERR("get mysql server ip error");
                goto outInstallIndex;
        }

        if (serverIp != ip && InstallGatewayAgent(&mysql, user, password, ip, serverIp) < 0) {
                LOG_ERR("install gwagent environment error");
                goto outInstallIndex;
        }
        // 安装分布式index节点
        if (!InstallStep(&mysql, ip, CL_FBS_INSTALL_INDEX(ip, user, password), "90", "install istri index...")) {
                LOG_ERR("install istri index error");
                goto outInstallIndex;
        }
        if (!InstallStep(&mysql, ip, CL_FBS_CLEAR_IDXROOTPATH(ip, user, password), "90", "clear idxrootpath...")) {
                LOG_ERR("update index server ip error");
                goto outInstallIndex;
        }
        if (!InstallStep(&mysql, ip, CL_FBS_SET_ISTRI_IDXIP(ip, user, password, ip), "100", "set index ip...")) {
                LOG_ERR("update index server ip error");
                goto outInstallIndex;
        }

        sql = SQL_SELECT_INDEX_NODE_ID(ip);
        if (mysql->Query(sql, md)) {
                if (md.Rows() > 0) {
                        LOG_WRN("istri index node %s already exist", ip.c_str());
                } else {
                        sql = SQL_INSERT_FBS_INDEX_NODE(ip, user, password, "ready");
                        if (!mysql->Query(sql)) {
                                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                                goto outInstallIndex;
                        }
                }
        } else {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                goto outInstallIndex;
        }

        delete mysql;
        return 0;
outInstallIndex:
        delete mysql;
        return -1;
}

int InstallMgr::InstallDataNode(const string &user, const string &password, const vector<string> &ipList, const string &indexId)
{
        thread t1(bind(&InstallMgr::StartInstallData, this, user, password, ipList, indexId));
        return 0;
}

int InstallMgr::StartInstallData(const string &user, const string &password, const vector<string> &ipList, const string &indexId)
{
        string sql, serverIp;
        MysqlClient* mysql = NULL;
        vector<string>::const_iterator vsIt;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                goto outInstallData;
        }

        if (!mysql->Query(SQL_TRUNCATE_FBS_INSTALL)) {
                LOG_ERR("mysql query error: sql=%s\n", SQL_TRUNCATE_FBS_INSTALL.c_str());
                goto outInstallData;
        }
        for (vsIt = ipList.begin(); vsIt != ipList.end(); ++vsIt) {
                string sql = SQL_INSERT_FBS_INSTALL(*vsIt, user, password, "data", indexId);
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        goto outInstallData;
                }
        }
        if (GetServerPortIp(serverIp) < 0) {
                LOG_ERR("get mysql server ip error");
                goto outInstallData;
        }

        for (vsIt = ipList.begin(); vsIt != ipList.end(); ++vsIt) {
                // 安装、启动网关代理
                if (InstallGatewayAgent(&mysql, user, password, *vsIt, serverIp) < 0) {
                        LOG_ERR("set gwagent environment error");
                        continue;
                }
                // 安装分布式data节点、配置idxServerIp、设置ID
                if (!InstallStep(&mysql, *vsIt, CL_FBS_INSTALL_DATA(*vsIt, user, password), "80", "install istriData...")) {
                        continue;
                }

                // set idxServerIp & data id
                MysqlData md;
                sql = SQL_SELECT_DATA_NODE_ID(*vsIt);
                if (!mysql->Query(sql, md)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        continue;
                }
                if (md.Rows() > 0) {
                        LOG_WRN("istri data node %s already exist", vsIt->c_str());
                        continue;
                }
                sql = SQL_INSERT_FBS_DATA_NODE(indexId, *vsIt, user, password, "ready");
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        continue;
                }
                sql = SQL_SELECT_INDEX_IP_AND_DATA_ID(*vsIt);
                if (!mysql->Query(sql, md)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        continue;
                }
                if (!InstallStep(&mysql, *vsIt, CL_FBS_SET_ISTRI_IDXIP(*vsIt, 
                        user, password, md.GetData(0,0)), "90", "set index ip...") ||
                    !InstallStep(&mysql, *vsIt, CL_FBS_CLEAR_DATAPATH(*vsIt, user, 
                        password), "90", "clear data path...") ||
                    !InstallStep(&mysql, *vsIt, CL_FBS_SET_ID(*vsIt, user, 
                        password, md.GetData(0,1)), "100", "set istri data id...")) {
                        LOG_ERR("set istri data id or set server ip on istri data error");
                        continue;
                }
        }
        delete mysql;
        return 0;
outInstallData:
        delete mysql;
        return -1;
}

int InstallMgr::UninstallStep(MysqlClient **mysql, const string &ip, const string &cmd, const string &process, const string &desc)
{
        vector<string> err;
        string sql = SQL_UPDATE_FBS_REMOVE(ip, process, desc, "waiting");
        if (!(*mysql)->Query(sql)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                return false;
        }
        if (Execute(cmd, err) != 0) {
                LOG_ERR("execute %s error %s", cmd.c_str(), err.back().c_str());
                sql = SQL_UPDATE_FBS_REMOVE(ip, process, desc, "error");
                if (!(*mysql)->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                }
                return false;
        }
        sql = SQL_UPDATE_FBS_REMOVE(ip, process, desc, "ok");
        if (!(*mysql)->Query(sql)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                return false;
        }
        return true;
}

int InstallMgr::RemoveIndexNode(const string &user, const string &password, const string &ip)
{
        vector<string> ipList;
        ipList.push_back(ip);
        thread t1(bind(&InstallMgr::StartRemoveIndex, this, user, password, ip));
        return 0;
}

int InstallMgr::StartRemoveIndex(const string &user, const string &password, const string &ip)
{
        // 0页面检查是否存在data
        // 1页面判断服务状态，仅停止状态下可以被删除
        // 2后台判断服务状态，仅停止状态下可以被删除
        // 3删除进度：正在检查服务状态 -> 正在卸载（gwagent, index）-> 清空数据库 -> 卸载网关 -> 完成

        int ret = 0;
        MysqlData md;
        string sql, cmd;
        string serverIp;
        vector<string> err;
        MysqlClient* mysql = NULL;

        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }
        // 将所有需要做连接测试的节点插入到临时表中
        if (!mysql->Query(SQL_TRUNCATE_FBS_REMOVE)) {
                LOG_ERR("mysql query error: sql=%s\n", SQL_TRUNCATE_FBS_REMOVE.c_str());
                goto outRemoveIndex;
        }
        sql = SQL_INSERT_FBS_REMOVE(ip, user, password, "index", "-1");
        if (!mysql->Query(sql)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                goto outRemoveIndex;
        }
        if (GetServerPortIp(serverIp) < 0) {
                LOG_ERR("get mysql server ip error");
                goto outRemoveIndex;
        }
        // get service status
        // uninstall istri index
        if (!UninstallStep(&mysql, ip, CL_FBS_ISTRIINDEX_STATUS(ip, user, password), "30", "get service state...") || 
            !UninstallStep(&mysql, ip, CL_FBS_UNINSTALL_INDEX(ip, user, password), "60", "uninstall istriIndex...")) {
                LOG_ERR("remove istriIndex error");
                goto outRemoveIndex;
        }

        // uninstall gwagent
        if (serverIp != ip && !UninstallStep(&mysql, ip, CL_FBS_UNINSTALL_GWAGENT(ip, user, password), "90", "uninstall gwagent...")) {
                LOG_ERR("remove gwagent error");
                goto outRemoveIndex;
        }

        // clear mysql databases
        sql = SQL_DELETE_FBS_INDEX_NODE(ip);
        if (!mysql->Query(sql)) {
                LOG_ERR("mysql query error: sql=%s", sql.c_str());
                goto outRemoveIndex;
        }
        sql = SQL_UPDATE_FBS_REMOVE(ip, "100", "clear database...", "ok");
        if (!mysql->Query(sql)) {
                LOG_ERR("mysql query error: sql=%s", sql.c_str());
                goto outRemoveIndex;
        }
        if (!mysql->Query(SQL_SELECT_INDEX_COUNT, md)) {
                LOG_ERR("mysql query error: ", SQL_SELECT_INDEX_COUNT.c_str());
                goto outRemoveIndex;
        }
        if (md.GetData(0,0) == "0") {
                if (Execute(CL_SET_SYSTEM_TYPE_SINGLE, err) < 0) {
                        LOG_ERR("execute %s error %s",
                                CL_SET_SYSTEM_TYPE_SINGLE.c_str(), err.back().c_str());
                        goto outRemoveIndex;
                }
        }

        delete mysql;
        return 0;
outRemoveIndex:
        delete mysql;
        return -1;
}

int InstallMgr::RemoveDataNode(const string &user, const string &password, const vector<string> &ipList, const string &indexId)
{
        thread t1(bind(&InstallMgr::StartRemoveData, this, user, password, ipList, indexId));
        return 0;
}

int InstallMgr::StartRemoveData(const string &user, const string &password, const vector<string> &ipList, const string &indexId)
{
        // 1 页面判断服务状态，仅停止状态下可以被删除
        // 2 后台判断服务状态，仅停止状态下可以被删除
        // 3 删除进度：正在检查服务状态 -> 正在卸载（gwagent, data）-> 清除ID -> 清空数据库 -> 卸载网关 -> 完成
        int ret = 0;
        string sql, cmd;
        vector<string> err;
        MysqlClient* mysql = NULL;
        vector<string>::const_iterator vsIt;

        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }
        // 将所有需要做连接测试的节点插入到临时表中
        if (!mysql->Query(SQL_TRUNCATE_FBS_REMOVE)) {
                LOG_ERR("mysql query error: sql=%s\n", SQL_TRUNCATE_FBS_REMOVE.c_str());
                goto outRemoveData;
        }
        for (vsIt = ipList.begin(); vsIt != ipList.end(); ++vsIt) {
                sql = SQL_INSERT_FBS_REMOVE(*vsIt, user, password, "data", indexId);
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        goto outRemoveData;
                }
        }

        // 查看服务是否停止，如果停止则卸载，否则跳过
        for (vsIt = ipList.begin(); vsIt != ipList.end(); ++vsIt) {
                if (!UninstallStep(&mysql, *vsIt, CL_FBS_ISTRIDATA_STATUS(*vsIt, 
                                user, password), "20", "get service state...") || 
                    !UninstallStep(&mysql, *vsIt, CL_FBS_UNINSTALL_DATA(*vsIt, 
                                user, password), "40", "uninstall istriData...") || 
                    !UninstallStep(&mysql, *vsIt, CL_FBS_UNINSTALL_GWAGENT(*vsIt, 
                                user, password), "60", "uninstall gwagent...")) {
                        LOG_ERR("remove istriData error");
                        continue;
                }
                MysqlData md, md1, md2;
                sql = SQL_SELECT_INDEX_IP_AND_DATA_ID(*vsIt);
                if (!mysql->Query(sql, md)) {
                        LOG_ERR("mysql query error: \n", sql.c_str());
                        continue;
                }
                sql = SQL_SELECT_DATA_STATUS(*vsIt);
                if (!mysql->Query(sql, md1)) {
                        LOG_ERR("mysql query error: \n", sql.c_str());
                        continue;
                }
                sql = SQL_SELECT_INDEX_NODE_INFO(md.GetData(0,0));
                if (!mysql->Query(sql, md2)) {
                        LOG_ERR("mysql query error: \n", sql.c_str());
                        continue;
                }
                if (md1.GetData(0,0) != "ready") {
                        if (!UninstallStep(&mysql, *vsIt, CL_FBS_CLEAR_ID(md.GetData(0,0), 
                                        user, md2.GetData(0,0), md.GetData(0,1)),"80", "clear id...")) {
                                sql = SQL_UPDATE_FBS_DATA_NODE(*vsIt, "unsetid");
                                if (!mysql->Query(sql)) {
                                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                                        continue;
                                }
                                continue;
                        }
                }

                // clear mysql databases
                sql = SQL_DELETE_FBS_DATA_NODE(*vsIt);
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        continue;
                }
                sql = SQL_UPDATE_FBS_REMOVE(*vsIt, "100", "clear database...", "ok");
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        continue;
                }
        }

        delete mysql;
        return 0;
outRemoveData:
        delete mysql;
        return -1;
}

int InstallMgr::GetServerPortIp(string &serverIp)
{
        vector<string> err;
        if (Execute(CL_GET_SERVER_IP, err) < 0) {
                LOG_ERR("execute [%s] error: %s", 
                        CL_GET_SERVER_IP.c_str(), err.back().c_str());
                return -1;
        }
        serverIp = err.back();
        return 0;
}
