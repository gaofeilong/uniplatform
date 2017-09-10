#include "IstriMgr.h"
#include "Include/Types.h"
#include "Include/String.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Config/IniConfig.h"
#include "Utils/Common/DirFileOpr.h"
#include "Utils/Common/CommonFunc.h"
#include "Server/Install//LicenseMgr.h"
#include "Server/Install/DdfsLicenseMgr.h"
#include "Server/Install/IstriLicenseMgr.h"
#include "Public/Mysql/Mysql.h"
#include "Public/Email/Email.h"
#include "Public/Config/ConfigReader.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>

using boost::bind;
using boost::thread;

IstriMgr::IstriMgr()
{ 
}

IstriMgr::~IstriMgr() 
{
}

int IstriMgr::StartIstriIndex(string &errInfo)
{
        vector<string> err;
        if (Execute(CL_FBS_ISTRI_INDEX_START, err) < 0) {
                errInfo = err[err.size() - 1];
                LOG_ERR("execute cmd error: %s", CL_FBS_ISTRI_INDEX_START);
                return -1;
        }
        return 0;
}

int IstriMgr::StartIstriData(string &errInfo)
{
        vector<string> err;
        if (Execute(CL_FBS_ISTRI_DATA_START, err) < 0) {
                errInfo = err[err.size() - 1];
                LOG_ERR("execute cmd error: %s", CL_FBS_ISTRI_DATA_START);
                return -1;
        }
        return 0;
}

int IstriMgr::StopIstriIndex(string &errInfo)
{
        vector<string> err;
        if (Execute(CL_FBS_ISTRI_INDEX_STOP, err) < 0) {
                errInfo = err[err.size() - 1];
                LOG_ERR("execute cmd error: %s", CL_FBS_ISTRI_INDEX_STOP);
                return -1;
        }
        return 0;
}

int IstriMgr::StopIstriData(string &errInfo)
{
        vector<string> err;
        if (Execute(CL_FBS_ISTRI_DATA_STOP, err) < 0) {
                errInfo = err[err.size() - 1];
                LOG_ERR("execute cmd error: %s", CL_FBS_ISTRI_DATA_STOP);
                return -1;
        }
        return 0;
}

int IstriMgr::FixIstriIndex(string &errInfo)
{
        vector<string> err;
        if (Execute(CL_FBS_ISTRI_INDEX_FIX, err) < 0) {
                errInfo = err[err.size() - 1];
                LOG_ERR("execute cmd error: %s", CL_FBS_ISTRI_INDEX_FIX);
                return -1;
        }
        return 0;
}

int IstriMgr::FixIstriData(string &errInfo)
{
        vector<string> err;
        if (Execute(CL_FBS_ISTRI_DATA_FIX, err) < 0) {
                errInfo = err[err.size() - 1];
                LOG_ERR("execute cmd error: %s", CL_FBS_ISTRI_DATA_FIX);
                return -1;
        }
        return 0;
}

int IstriMgr::FirstEditIstriData(const string &mp, const string &indexPath, 
                const string &metaPath, const vector<string> &dataPath, const string &isImgDedup)
{
        string allNewPath;
        vector<string> err;
        if (!InitPath(mp)) {
                LOG_ERR("init mountPoint %s error", mp.c_str());
                return -1;
        }
        if (!InitPath(indexPath)) {
                LOG_ERR("init indexPath %s error", indexPath.c_str());
                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                        LOG_ERR("execute cmd %s error", CL_DEL_PATH(allNewPath).c_str());
                }
                return -1;
        }
        allNewPath += indexPath + " ";
        if (!InitPath(metaPath)) {
                LOG_ERR("init metaPath %s error", metaPath.c_str());
                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                        LOG_ERR("execute cmd %s error", CL_DEL_PATH(allNewPath).c_str());
                }
                return -1;
        }
        allNewPath += metaPath + " ";

        vector<string> oldEmptyDataPath;
        if (EditIstriMultiPath(oldEmptyDataPath, dataPath) != 0) {
                LOG_ERR("init dataPath error");
                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                        LOG_ERR("execute cmd %s error", CL_DEL_PATH(allNewPath).c_str());
                }
                return -1;
        }

        IniConfig ini(DEFAULT_FBS_CONF);
        if (ini.Open() != 0) {
                LOG_ERR("open config %s error", DEFAULT_FBS_CONF);
                return -1;
        }
        if (ini.SetVal("FileSystem", "mountPoint", mp) != 0 ||
            ini.SetVal("Lds", "indexPath", indexPath) != 0 ||
            ini.SetVal("MetaData", "metaPath", metaPath) != 0 || 
            ini.SetVal("FileSystem", "isPhoto", isImgDedup) != 0) {
                LOG_ERR("set istri mp, index, meta error ");
                return -1;
        }
        for (size_t i = 1; i <= dataPath.size(); i++) {
                if (ini.SetVal("Lds", string("dataPath") + 
                        IntToString(i), dataPath[i - 1]) != 0) {
                        LOG_ERR("set istri datapath error ");
                        return -1;
                }
        }
        if (ini.Close() != 0) {
                LOG_ERR("close config %s error", DEFAULT_FBS_CONF);
                return -1;
        }

        return 0;
}

int IstriMgr::EditIstriData(const vector<string> &oldDataPath, const vector<string> &newDataPath)
{
        if (EditIstriMultiPath(oldDataPath, newDataPath) < 0) {
                LOG_ERR("init dataPath error");
                return -1;
        }
        vector<string> err;
        string status = DataServiceStatus();
        if (status == "running") {
                vector<string>::const_iterator vsCit;
                for (vsCit = newDataPath.begin(); vsCit != newDataPath.end(); ++vsCit) {
                        if (Execute(CL_FBS_HOTPLUG_DATA_PATH(*vsCit), err) < 0) {
                                LOG_ERR("execute cmd %s error!", 
                                        CL_FBS_HOTPLUG_DATA_PATH(*vsCit).c_str());
                                return -1;
                        }
                }
        } else if (status == "stopped") {
                IniConfig ini(DEFAULT_FBS_CONF);
                if (ini.Open() != 0) {
                        LOG_ERR("open config %s error", DEFAULT_FBS_CONF);
                        return -1;
                }
                size_t i = oldDataPath.size() + 1; 
                vector<string>::const_iterator vsCit;
                for (vsCit = newDataPath.begin(); vsCit != newDataPath.end(); ++vsCit) {
                        if (ini.SetVal("Lds", string("dataPath") + IntToString(i++), *vsCit) != 0) {
                                LOG_ERR("set istri datapath %s error", vsCit->c_str());
                                return -1;
                        }
                }
                if (ini.Close() != 0) {
                        LOG_ERR("close config %s error", DEFAULT_FBS_CONF);
                        return -1;
                }
        }
        return 0;
}

int IstriMgr::EditIstriIndex(const vector<string> &oldIdxRootPath, const vector<string> &newIdxRootPath)
{
        if (EditIstriMultiPath(oldIdxRootPath, newIdxRootPath) < 0) {
                LOG_ERR("edit istri idxRootPaths error");
                return -1;
        }

        vector<string> err;
        string status = IndexServiceStatus();
        if (status == "running") {
                vector<string>::const_iterator vsCit;
                for (vsCit = newIdxRootPath.begin(); vsCit != newIdxRootPath.end(); ++vsCit) {
                        if (Execute(CL_FBS_HOTPLUG_INDEX_PATH(*vsCit), err) < 0) {
                                LOG_ERR("execute cmd %s error!", 
                                        CL_FBS_HOTPLUG_INDEX_PATH(*vsCit).c_str());
                                return -1;
                        }
                }
        } else if (status == "stopped") {
                IniConfig ini(DEFAULT_FBS_CONF);
                if (ini.Open() != 0) {
                        LOG_ERR("open config %s error", DEFAULT_FBS_CONF);
                        return -1;
                }
                size_t i = oldIdxRootPath.size();
                vector<string>::const_iterator vsCit;
                for (vsCit = newIdxRootPath.begin(); vsCit != newIdxRootPath.end(); ++vsCit) {
                        if (ini.SetVal("IdxVolumePath", string("idxRootPath") + IntToString(i++), *vsCit) != 0) {
                                LOG_ERR("set istri idxRootPath %s error", vsCit->c_str());
                                return -1;
                        }
                }
                if (ini.SetVal("IdxVolumePath", "count", IntToString(i)) != 0) {
                        LOG_ERR("set istri idxRootPath count %d error", i);
                        return -1;
                }
                if (ini.Close() != 0) {
                        LOG_ERR("close config %s error", DEFAULT_FBS_CONF);
                        return -1;
                }
        }
        return 0;
}

int IstriMgr::EditIstriMultiPath(const vector<string> &oldPath, const vector<string> &newPath)
{
        string allNewPath;
        vector<string> err;
        vector<string>::const_iterator vsItOld, vsItNew;
        for (vsItNew = newPath.begin(); vsItNew != newPath.end(); ++vsItNew) {
                if (!InitPath(*vsItNew)) {
                        LOG_ERR("init newPath %s error", vsItNew->c_str());
                        if (!allNewPath.empty() && Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                                LOG_ERR("execute cmd %s error", CL_DEL_PATH(allNewPath).c_str());
                        }
                        return -1;
                }
                for (vsItOld = newPath.begin(); vsItOld != vsItNew; ++vsItOld) {
                        string cmd = CL_DIFF_DEVICE(*vsItNew, *vsItOld);
                        if (Execute(CL_DIFF_DEVICE(*vsItNew, *vsItOld), err) < 0) {
                                LOG_ERR("execute cmd %s error", CL_DIFF_DEVICE(*vsItNew, *vsItOld).c_str());
                                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                                        LOG_ERR("execute cmd %s error", CL_DEL_PATH(allNewPath).c_str());
                                }
                                return -1;
                        }
                        if (err.size() == 1) {
                                LOG_ERR("%s and %s has same st_dev", vsItNew->c_str(), vsItOld->c_str());
                                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                                        LOG_ERR("execute cmd %s error", CL_DEL_PATH(allNewPath).c_str());
                                }
                                return -1;
                        }
                }

                allNewPath += *vsItNew + " ";
                for (vsItOld = oldPath.begin(); vsItOld != oldPath.end(); ++vsItOld) {
                        string cmd = CL_DIFF_DEVICE(*vsItNew, *vsItOld);
                        if (Execute(CL_DIFF_DEVICE(*vsItNew, *vsItOld), err) < 0) {
                                LOG_ERR("execute cmd %s error", CL_DIFF_DEVICE(*vsItNew, *vsItOld).c_str());
                                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                                        LOG_ERR("execute cmd %s error", CL_DEL_PATH(allNewPath).c_str());
                                }
                                return -1;
                        }
                        if (err.size() == 1) {
                                LOG_ERR("%s and %s has same st_dev", vsItNew->c_str(), vsItOld->c_str());
                                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                                        LOG_ERR("execute cmd %s error", CL_DEL_PATH(allNewPath).c_str());
                                }
                                return -1;
                        }
                }
        }
        return 0;
}

int IstriMgr::ClearIstriDataId(const string &id, string &errInfo)
{
        vector<string> err;
        MysqlClient* mysql = NULL;

        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }
        if (Execute(CL_FBS_CLEAR_DATA_ID(id), err) < 0) {
                errInfo = err[err.size() - 1];
                LOG_ERR("execute cmd error: %s", CL_FBS_ISTRI_DATA_STOP);
                // delete mysql;        // 失败，可能是节点没有被启动过，不退出
                // return -1;
        }

        if (!mysql->Query(SQL_DELETE_FBS_DATA_NODE_FROM_ID(id))) {
                LOG_ERR("mysql query error: sql=%s\n", SQL_DELETE_FBS_DATA_NODE_FROM_ID(id).c_str());
                delete  mysql;
                return -1;
        }

        delete  mysql;
        return 0;
}

// int IstriMgr::GetGatewayConfig(struct _GatewayConfig &c)
// {
//         ConfigReader<struct _GatewayConfig> gatewayConfigReader(SCIGW_CONF);
//         if (gatewayConfigReader.Read(c) < 0) {
//                 LOG_ERR("read gateway config %s error", SCIGW_CONF);
//                 return -1;
//         }
//         return 0;
// }

int IstriMgr::InitNodeState(map<string, struct NodeState> &nodeStateList)
{
        MysqlData md;
        MysqlDataRow mdr;
        MysqlClient* mysql = NULL;
        string ip, state, table;

        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }

        nodeStateList.clear();
        if (!mysql->Query(SQL_SELECT_INDEX_NODE_STATE, md)) {
                LOG_ERR("mysql query error: %s\n", SQL_SELECT_INDEX_NODE_STATE.c_str());
                delete mysql;
                return -1;
        }
        for (size_t i = 0; i < md.Rows(); i++) {
                mdr = md.GetRow(i);
                // LOG_DBG("%s", mdr["updatetime"].c_str());
                // LOG_DBG("%ld", TimeStrToUtcSec(mdr["updatetime"]));
                // LOG_DBG("%s", UtcSecToTimeStr(TimeStrToUtcSec(mdr["updatetime"])).c_str());
                time_t time = TimeStrToUtcSec(mdr["updatetime"]);
                nodeStateList.insert(make_pair<string, NodeState>(mdr["ip"],
                        NodeState(mdr["ip"], time, mdr["state"], "fbs_index_node")));
                // LOG_DBG("ip[%s], time[%s(%ld)], state[%s], table[%s]", mdr["ip"].c_str(), 
                //         mdr["updatetime"].c_str(), time, mdr["state"].c_str(), "fbs_index_node");
        }

        if (!mysql->Query(SQL_SELECT_DATA_NODE_STATE, md)) {
                LOG_ERR("mysql query error: \n", SQL_SELECT_DATA_NODE_STATE.c_str());
                delete mysql;
                return -1;
        }
        for (size_t i = 0; i < md.Rows(); i++) {
                mdr = md.GetRow(i);
                // LOG_DBG("%s", mdr["updatetime"].c_str());
                // LOG_DBG("%ld", TimeStrToUtcSec(mdr["updatetime"]));
                // LOG_DBG("%s", UtcSecToTimeStr(TimeStrToUtcSec(mdr["updatetime"])).c_str());
                time_t time = TimeStrToUtcSec(mdr["updatetime"]);
                nodeStateList.insert(make_pair<string, NodeState>(mdr["ip"],
                        NodeState(mdr["ip"], time, mdr["state"], "fbs_data_node")));
                // LOG_DBG("ip[%s], time[%s(%ld)], state[%s], table[%s]", mdr["ip"].c_str(), 
                //         mdr["updatetime"].c_str(), time, mdr["state"].c_str(), "fbs_index_node");
        }

        delete mysql;
        return 0;
}

int IstriMgr::SetNodeStateLost(const string &ip, const string &table)
{
        MysqlClient* mysql = NULL;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }
        if (!mysql->Query(SQL_UPDATE_NODE_STATE_LOST(ip, table))) {
                LOG_ERR("mysql query error: \n", 
                        SQL_UPDATE_NODE_STATE_LOST(ip, table).c_str());
                delete mysql;
                return -1;
        }
        delete mysql;
        return 0;
}

// 服务器节点上用于监控所有index、data节点是否断开连接的线程
int IstriMgr::UpdateNodeState()
{
        struct _GatewayConfig gwConfig;
        map<string, struct NodeState> oldState, newState;
        map<string, struct NodeState>::iterator msnIt, target;
        ConfigReader<struct _GatewayConfig> gatewayConfigReader(SCIGW_CONF);
        if (gatewayConfigReader.Read(gwConfig) < 0) {
                LOG_ERR("read gateway config %s error", SCIGW_CONF);
                return -1;
        }
        time_t interval = atol(gwConfig.updateInterval.c_str());
        map<string, bool> nodeAlarmState;

        while (1) {
                if (oldState.empty()) {
                        LOG_DBG("empty old list");
                        if (InitNodeState(oldState) < 0) {
                                LOG_ERR("init node state error");
                        }
                        goto goon;
                }

                if (InitNodeState(newState) < 0) {
                        LOG_ERR("get node state error");
                        goto goon;
                }
                // 循环检查旧的已存在记录，对于每一个记录，如果在新节点状态
                // 列表中找不到（说明节点已经在上一个循环周期中删除）或者节
                // 点的更新时间戳>=上次的时间戳+间隔时间（说明正常更新）则
                // 接续测试下一个节点记录。否则将节点列表中状态设置为lost
                // （如果之前的状态不是lost）或者将数据库中的节点状态置位
                // lost（如果之前的节点状态已经是lost）

                // for (msnIt = oldState.begin(); msnIt != oldState.end(); ++msnIt) {
                //         LOG_DBG("OLD: ip[%s], time[%s(%ld)], state[%s], table[%s]", 
                //                 msnIt->first.c_str(), UtcSecToTimeStr(msnIt->second.Time).c_str(), 
                //                 msnIt->second.Time, msnIt->second.State.c_str(), msnIt->second.Table.c_str());
                // }
                // for (msnIt = newState.begin(); msnIt != newState.end(); ++msnIt) {
                //         LOG_DBG("NEW: ip[%s], time[%s(%ld)], state[%s], table[%s]", 
                //                 msnIt->first.c_str(), UtcSecToTimeStr(msnIt->second.Time).c_str(), 
                //                 msnIt->second.Time, msnIt->second.State.c_str(), msnIt->second.Table.c_str());
                // }

                for (msnIt = oldState.begin(); msnIt != oldState.end(); ++msnIt) {
                        target = newState.find(msnIt->first);
                        // 节点被删除或者被更新了
                        if (target == newState.end() || target->second.Time != msnIt->second.Time) {
                                if (nodeAlarmState[msnIt->first]) {
                                        LOG_INF("regain or remove lost node, ip[%s]", target->first.c_str());
                                        nodeAlarmState.erase(target->first);
                                }
                                continue;
                        }
                        // 节点已经被报警过
                        if (nodeAlarmState[target->first]) {
                                continue;
                        } 
                        LOG_WRN("lost: ip[%s], time[%s(%ld)], state[%s], interval[%d]", 
                                msnIt->first.c_str(), 
                                UtcSecToTimeStr(msnIt->second.Time).c_str(), msnIt->second.Time, 
                                target->second.State.c_str(), interval);

                        if (SetNodeStateLost(target->first, target->second.Table) < 0) {
                                LOG_ERR("set node %s state lost error", target->first.c_str());
                        }
                        Mail* mail = GetEmailObj("warning");
                        if (mail == NULL) {
                                LOG_DBG("cannot get mail obj");
                        } else {
                                string type = target->second.Table == "fbs_index_node"? "index": "data";
                                string errInfo = "\tlost istri " + type + " node: " + target->first + "\n";
                                mail->SendMail("WARNING: istri node lost", errInfo);
                                nodeAlarmState[target->first] = true;
                                delete mail;
                        }
                } 
                oldState = newState;
goon:
                // 为了防止偶尔连接数据库失败的情况，这里更新间隔放大
                sleep(atol(gwConfig.updateInterval.c_str()) * 3);
        }
}

// index、data节点上更新节点信息的线程
int IstriMgr::UpdateNodeInfo()
{
        struct _GatewayConfig gwConfig;
        ConfigReader<struct _GatewayConfig> gatewayConfigReader(SCIGW_CONF);
        if (gatewayConfigReader.Read(gwConfig) < 0) {
                LOG_ERR("read gateway config %s error", SCIGW_CONF);
                return -1;
        }

        while (1) {
                if (access(FBS_INDEX_DAEMON, F_OK) == 0) {
                        if (UpdateIndexInfo() < 0) {
                                LOG_ERR("update index node info error");
                        }
                } else if (access(FBS_DATA_DAEMON, F_OK) == 0) {
                        if (UpdateDataInfo() < 0) {
                                LOG_ERR("update data node info error");
                        }
                } else if (access(MKFS_DDFS_FILE, F_OK) == 0 &&
                           access(MOUNT_DDFS_FILE, F_OK) == 0 &&
                           access(FSCK_DDFS_FILE, F_OK) == 0) {
                        if (UpdateDdfsInfo() < 0) {
                                LOG_ERR("update ddfs node info error");
                        }
                } else {
                        LOG_DBG("neither ddfs nor istri installed");
                }
                sleep(atol(gwConfig.updateInterval.c_str()));
                continue;
        }
}

int IstriMgr::UpdateIndexInfo()
{
        // get istri index node ip
        vector<string> err;
        if (Execute(CL_FBS_GET_INDEXSERVERIP, err) < 0) {
                LOG_ERR("execute cmd %s error", CL_FBS_GET_INDEXSERVERIP.c_str());
                return -1;
        }
        string ip = err[0];

        MysqlData md;
        MysqlClient* mysql = NULL;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }
        string sql = SQL_SELECT_INDEX_NODE_STATE_ONLY(ip);
        if (!mysql->Query(sql, md)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                delete mysql;
                return -1;
        }
        if (md.GetData(0,0) == "ready") {
                string status = "ready";
                sql = SQL_UPDATE_FBS_INDEX_NODE_STATE(ip, GetCurSec(), status);
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        delete mysql;
                        return -1;
                }
        } else {
                // get idxRootPaths' list
                if (Execute(CL_FBS_GET_IDXROOTPATH, err) < 0) {
                        LOG_ERR("execute cmd %s error", CL_FBS_GET_IDXROOTPATH.c_str());
                        delete mysql;
                        return -1;
                }
                string paths = err[0];

                // get idxRootPaths' capacity, used/ left
                if (Execute(CL_FBS_GET_INDEX_USAGE(paths), err) < 0) {
                        LOG_ERR("execute cmd %s error", CL_FBS_GET_INDEX_USAGE(err[0]).c_str());
                        delete mysql;
                        return -1;
                }
                string capacity = err[0];
                string used = err[1];
                string left = err[2];
                string status = capacity == "0"? "ready": IndexServiceStatus();

                // update to mysql databases
                sql = SQL_UPDATE_FBS_INDEX_NODE_USAGE(ip, capacity, used, left, GetCurSec(), status);
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        delete mysql;
                        return -1;
                }

                static bool isSpaceAlarmed = false;
                if ((double)atol(used.c_str()) / atol(capacity.c_str()) > 0.8) {
                        Mail* mail = GetEmailObj("warning");
                        if (mail == NULL) {
                                LOG_DBG("cannot get mail obj");
                        } else if (!isSpaceAlarmed) {
                                string errInfo = "istri index node " + ip + 
                                        ": \n\tless than 20 percent storage space left in idxRootPath";
                                mail->SendMail("WARNING: not enough space on disk", errInfo);
                                isSpaceAlarmed = true;
                                delete mail;
                        }
                } else {
                        isSpaceAlarmed = false;
                }
        }

        BOOSTPTR<LicenseMgr> lic;
        if (!mysql->Query(SQL_SELECT_INDEX_NODE_INFO(ip), md)) {
                LOG_ERR("mysql query error: \n", SQL_SELECT_INDEX_NODE_INFO(ip).c_str());
                delete mysql;
                return -1;
        }
        if (md.Rows() != 0) {
                lic = BOOSTPTR<LicenseMgr>(new IstriLicenseMgr(ip, "root", md.GetData(0,0)));
        } else {
                LOG_INF("cannot find index node: %s", ip.c_str());
        } 
        delete mysql;

        static int oldIstrilarmState = 0;
        string info = "license information for istri index node: " + ip + "\n";
        if (CheckLicense(lic, info, oldIstrilarmState) < 0) {
                LOG_ERR("check istri %s license info error", ip.c_str());
                return -1;
        }
        return 0;
}

int IstriMgr::UpdateDataInfo()
{
        // get istri data node id
        vector<string> err;
        if (Execute(CL_FBS_GET_DATA_ID, err) < 0) {
                LOG_ERR("execute cmd %s error", CL_FBS_GET_DATA_ID.c_str());
                return -1;
        }
        string id = err[0];

        MysqlData md;
        MysqlClient* mysql = NULL;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }

        // get data status in database
        string sql = SQL_SELECT_DATA_NODE_STATE_ONLY(id);
        if (!mysql->Query(sql, md)) {
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                delete mysql;
                return -1;
        }
        string statusInDb = md.GetData(0,0);
        // get dataPaths' list
        if (Execute(CL_FBS_GET_DATAPATH, err) < 0) {
                LOG_ERR("execute cmd %s error", CL_FBS_GET_DATAPATH.c_str());
                delete mysql;
                return -1;
        }
        string paths = err[0];
        // get dataPaths' capacity, used, left and data node's total, real, ratio
        if (Execute(CL_FBS_GET_DATA_USAGE(paths), err) < 0) {
                LOG_ERR("execute cmd %s error", CL_GET_PATH_CAPACITY(err[0]).c_str());
                delete mysql;
                return -1;
        }
        string capacity = err[0];
        string used = err[1];
        string left = err[2];
        string total = IntToString(UserToDigit(err[3]));
        string real = IntToString(UserToDigit(err[4]));
        string dedup = err[5];
        string status = capacity == "0"? "ready": DataServiceStatus();

        if (status == "running") {
                // update to mysql databases
                sql = SQL_UPDATE_FBS_DATA_NODE_USAGE(id, capacity, used, 
                                left, total, real, dedup, GetCurSec(), status);
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        delete mysql;
                        return -1;
                }

                // warning email 
                string sql = SQL_SELECT_DATA_NODE_IP(id);
                if (!mysql->Query(sql, md)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        delete mysql;
                        return -1;
                }
                static bool isSpaceAlarmed = false;
                if ((double)atol(used.c_str()) / atol(capacity.c_str()) > 0.8) {
                        Mail* mail = GetEmailObj("warning");
                        if (mail == NULL) {
                                LOG_DBG("cannot get mail obj");
                        } else if (!isSpaceAlarmed) {
                                string errInfo = "istri data node " + md.GetData(0,0) + "(" + id + 
                                        "): \n\tless than 20 percent storage space left in dataPath";
                                mail->SendMail("WARNING: not enough space on disk", errInfo);
                                isSpaceAlarmed = true;
                                delete mail;
                        }
                } else {
                        isSpaceAlarmed = false;
                }
        } else {
                sql = SQL_UPDATE_FBS_DATA_NODE_STATE(id, GetCurSec(), status);
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        delete mysql;
                        return -1;
                }
        }
        delete mysql;
        return 0;
}

int IstriMgr::UpdateDdfsInfo()
{
        BOOSTPTR<LicenseMgr> lic = BOOSTPTR<LicenseMgr>(new DdfsLicenseMgr);
        string info = "license information for ddfs:\n";
        static int oldDdfsAlarmState = 0;

        if (CheckLicense(lic, info, oldDdfsAlarmState) < 0) {
                LOG_ERR("check ddfs license information error");
        }
        return 0;
}

bool IstriMgr::InitPath(const string &path)
{
        DirFileOpr dfOpr;
        if (dfOpr.HasPath(path)) {
                if (!dfOpr.IsDir(path) || !dfOpr.IsDirEmpty(path)) {
                        LOG_ERR("%s is not an empty dir", path.c_str());
                        return false;
                }
        } else {
                if (dfOpr.MakeDir(path) != 0) {
                        LOG_ERR("create directory %s error", path.c_str());
                        return false;
                }
        }
        return true;
}

string IstriMgr::IndexServiceStatus()
{
        return ServiceStatus(CL_FBS_ISTRI_INDEX_STATUS);
}

string IstriMgr::DataServiceStatus()
{
        return ServiceStatus(CL_FBS_ISTRI_DATA_STATUS);
}

string IstriMgr::ServiceStatus(const string &cmd)
{
        vector<string> err;
        Execute(cmd, err);
        if (err[err.size() - 1] == "Service is running") {
                return "running";
        } else if (err[err.size() - 1] == "Service is stopped") {
                return "stopped";
        } else {
                return "error";
        }
}

int IstriMgr::CheckLicense(BOOSTPTR<LicenseMgr> &lic, const string &info, int &oldAlarmState)
{
        string mailInfo = info;
        int newAlarmState = 0;
        int ret = lic->CheckLicense();
        if (ret > 0) {
                if (ret & QUOTA_INVALID) {
                        mailInfo += "\tlicense quota less than 20 percent\n";
                        newAlarmState |= QUOTA_INVALID;
                }
                if (ret & TIME_INVALID || ret & ABSTIME_INVALID) {
                        mailInfo += "\tlicense time less than 30 days\n";
                        newAlarmState |= TIME_INVALID;
                }
                Mail* mail = GetEmailObj("warning");
                if (mail == NULL) {
                        LOG_DBG("cannot get mail obj");
                } else if (newAlarmState != oldAlarmState) {
                        mail->SendMail("WARNING: license authorization going to be invalid", mailInfo);
                        oldAlarmState = newAlarmState;
                        delete mail;
                }
        } else {
                oldAlarmState = 0;
        }

        return 0;
}
