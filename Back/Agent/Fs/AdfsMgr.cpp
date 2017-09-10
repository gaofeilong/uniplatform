/*******************************************************************************
 *
 * @ File Name  : AdfsMgr.cpp
 * @ Date       : 2013-10-29
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: Adfs 文件系统管理
 * @ History    : 2013-10-29：创建
 *
 * ****************************************************************************/

#include "AdfsMgr.h"
#include "Include/String.h"
#include "Public/Mysql/Mysql.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Config/IniConfig.h"
#include "Utils/Common/CommonFunc.h"

AdfsMgr::AdfsMgr()
{ 
}

AdfsMgr::~AdfsMgr() 
{
}

// int64_t capacity;
// int64_t left;
// int64_t total;
// int64_t real;
// double  ratio;
// string  state;
int AdfsMgr::GetMpList(vector<struct _AdfsInfo> &mpList)
{
        if (GetAllDataPath(mpList) < 0) {
                LOG_ERR("get all mp dataPath error");
                return -1;
        }

        vector<string> err;
        string cmd, dataPathStr;
        vector<struct _AdfsInfo>::iterator vaIt = mpList.begin();

        for (; vaIt != mpList.end(); ++vaIt) {
                // state
                vaIt->state = GetMpStatus(vaIt->firstDataPath + "/mp");

                // capacity and left
                dataPathStr = vaIt->firstDataPath;
                vector<string>::iterator vsIt = vaIt->otherDataPath.begin();
                for (; vsIt != vaIt->otherDataPath.end(); ++vsIt) {
                        dataPathStr += " " + *vsIt;
                }
                cmd = CL_GET_PATH_CAPACITY(dataPathStr);
                if (Execute(cmd, err) < 0) {
                        LOG_ERR("execute cmd[%s] error: %s", 
                                cmd.c_str(), err.back().c_str());
                        return -2;
                }
                vaIt->capacity = err[0];
                vaIt->left     = err[2];

                // total, real and ratio
                if (vaIt->state == "mounted") {
                        cmd = CL_GET_ADFS_MP_TOTAL(vaIt->firstDataPath);
                        if (Execute(cmd, err) < 0) {
                                LOG_ERR("execute cmd[%s] error: %s", 
                                        cmd.c_str(), err.back().c_str());
                                return -3;
                        }
                        vaIt->total = err[0];

                        cmd = CL_GET_ADFS_MP_REAL(vaIt->firstDataPath);
                        if (Execute(cmd, err) < 0) {
                                LOG_ERR("execute cmd[%s] error: %s", 
                                        cmd.c_str(), err.back().c_str());
                                return -4;
                        }
                        vaIt->real = err[0];

                        cmd = CL_GET_ADFS_MP_RATIO(vaIt->firstDataPath);
                        if (Execute(cmd, err) < 0) {
                                LOG_ERR("execute cmd[%s] error: %s", 
                                        cmd.c_str(), err.back().c_str());
                                return -5;
                        }
                        vaIt->ratio = err[0];
                } else {
                        vaIt->total = "0";
                        vaIt->real  = "0";
                        vaIt->ratio = "0";
                }
        }

        return 0;
}

int AdfsMgr::Create(const string &firstDataPath, 
                    const vector<string> otherDataPath)
{
        vector<struct _AdfsInfo> mpList, mpListBak;
        if (GetAllDataPath(mpList) < 0) {
                LOG_ERR("get all mp dataPath error");
                return -1;
        }
        mpListBak = mpList;

        // 将新增的挂载点配置和原有的出来
        vector<string>::iterator vsIt;
        vector<string> newPathList, oldPathList;

        newPathList.assign(otherDataPath.begin(), otherDataPath.end());
        newPathList.push_back(firstDataPath);
        for (size_t i = 0; i < mpList.size(); ++i) {
                oldPathList.push_back(mpList[i].firstDataPath);
                oldPathList.insert(oldPathList.end(), 
                                   mpList[i].otherDataPath.begin(), 
                                   mpList[i].otherDataPath.end());
        }

        // 创建新增挂载点必要的目录
        // 构造数据路径列表字符串
        struct _AdfsInfo adfsInfo;
        vector<string> tmpNewPathList;
        string dataPathStr = firstDataPath;
        adfsInfo.firstDataPath = firstDataPath;

        for (vsIt = newPathList.begin(); vsIt != newPathList.end(); ++vsIt) {
                if (!InitPath(*vsIt)) {
                        LOG_ERR("init path %s error", vsIt->c_str());
                        return -2;
                }
                if (*vsIt != firstDataPath) {
                        dataPathStr += ":" + *vsIt;
                        adfsInfo.otherDataPath.push_back(*vsIt);
                }

                // 做分区唯一性判断
                tmpNewPathList.clear();
                tmpNewPathList.push_back(*vsIt);
                if (!IsPathListValid(tmpNewPathList, oldPathList)) {
                        LOG_ERR("path list of created mp %s is invalid", 
                                        (firstDataPath + "/mp").c_str());
                        return -4;
                }
                oldPathList.push_back(*vsIt);
        }
        mpList.push_back(adfsInfo);

        // 将修改的配置写回配置文件
        if (SetAllDataPath(mpList) < 0) {
                LOG_ERR("set all mp dataPath error");
                return -4;
        }

        // 挂载点初始化
        string key;
        if (GetDataPathKey(firstDataPath, key) < 0) {
                LOG_ERR("get key of datapath %s error", firstDataPath.c_str());
                return -5;
        }
        vector<string> err;
        string cmd = CL_ADFS_MKFS(key);
        if (Execute(cmd, err) < 0) {
                LOG_ERR("execute cmd[%s] error: %s", 
                        cmd.c_str(), err.back().c_str());
                if (SetAllDataPath(mpListBak) < 0) {
                        LOG_ERR("retrieve adfs config error");
                }
                return -3;
        }

        // 同步数据库中的配置信息
        MysqlClient* mysql = NULL;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -5;
        }
        string sql = SQL_INSERT_ADFS_MP(firstDataPath, dataPathStr);
        if (!mysql->Query(sql)) {
                LOG_ERR("execute sql: %s error", sql.c_str());
                delete mysql;
                return -6;
        }
        delete mysql;

        return 0;
}

int AdfsMgr::Modify(const string &firstDataPath, 
                    const vector<string> newDataPath)
{
        vector<struct _AdfsInfo> mpList;
        if (GetAllDataPath(mpList) < 0) {
                LOG_ERR("get all mp dataPath error");
                return -1;
        }

        // 查找到要编辑MP那一项
        // 记录所有新增路径和已存在的路径
        int key = -1;
        vector<string> oldPathList, tmpNewPathList;
        for (size_t i = 0; i < mpList.size(); i++) {
                if (mpList[i].firstDataPath == firstDataPath) {
                        key = i;
                } 
                oldPathList.push_back(mpList[i].firstDataPath);
                oldPathList.insert(oldPathList.end(), 
                                   mpList[i].otherDataPath.begin(),
                                   mpList[i].otherDataPath.end());
        }
        if (-1 == key) {
                LOG_ERR("cannot find adfsmp");
                return -2;
        }
        
        // 创建需要的路径，并将新增的路径配置插入到对应要修改的项的数组中
        vector<string>::const_iterator vsIt = newDataPath.begin();
        for (; vsIt != newDataPath.end(); ++vsIt) {
                if (!InitPath(*vsIt)) {
                        LOG_ERR("init path %s error", vsIt->c_str());
                        return -3;
                }
                mpList[key].otherDataPath.push_back(*vsIt);

                tmpNewPathList.clear();
                tmpNewPathList.push_back(*vsIt);
                if (!IsPathListValid(tmpNewPathList, oldPathList)) {
                        LOG_ERR("path list of created mp %s is invalid", 
                                        (firstDataPath + "/mp").c_str());
                        return -4;
                }
                oldPathList.push_back(*vsIt);
        }

        // 获取修改后的数据路径字符串（去掉结尾的冒号），用于插入数据库
        string dataPathStr;
        vsIt = mpList[key].otherDataPath.begin();
        for (; vsIt != mpList[key].otherDataPath.end(); ++vsIt) {
                dataPathStr += *vsIt + ":";
        }
        dataPathStr = dataPathStr.substr(0, dataPathStr.size() - 1);

        // 将修改的配置写回配置文件
        if (SetAllDataPath(mpList) < 0) {
                LOG_ERR("set all mp dataPath error");
                return -5;
        }

        // 同步数据库中的配置信息
        MysqlClient* mysql = NULL;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -6;
        }
        string sql = SQL_UPDATE_ADFS_MP(firstDataPath, dataPathStr);
        if (!mysql->Query(sql)) {
                LOG_ERR("execute sql: %s error", sql.c_str());
                delete mysql;
                return -7;
        }
        delete mysql;

        return 0;
}

int AdfsMgr::Mount(const string &firstDataPath)
{
        vector<struct _AdfsInfo> mpList;
        if (GetAllDataPath(mpList) < 0) {
                LOG_ERR("get all mp dataPath error");
                return -1;
        }

        string dataPathStr;
        for (size_t i = 0; i < mpList.size(); ++i) {
                if (mpList[i].firstDataPath != firstDataPath) {
                        continue;
                }
                dataPathStr = mpList[i].firstDataPath;
                for (size_t j = 0; j < mpList[i].otherDataPath.size(); ++j) {
                        dataPathStr += ":" + mpList[i].otherDataPath[j];
                }
                break;
        }
        if (dataPathStr.empty()) {
                LOG_ERR("cannot find adfsmp");
                return -2;
        }

        string key;
        if (GetDataPathKey(firstDataPath, key) < 0) {
                LOG_ERR("get key of datapath %s error", firstDataPath.c_str());
                return -5;
        }
        vector<string> err;
        string cmd = CL_ADFS_MOUNT(key);
        if (Execute(cmd, err) < 0) {
                LOG_ERR("execute cmd[%s] error: %s", 
                        cmd.c_str(), err.back().c_str());
                return -3;
        }
        return 0;
}

int AdfsMgr::Umount(const string &firstDataPath)
{
        string key;
        if (GetDataPathKey(firstDataPath, key) < 0) {
                LOG_ERR("get key of datapath %s error", firstDataPath.c_str());
                return -5;
        }
        vector<string> err;
        string cmd = CL_ADFS_UMOUNT(key);
        if (Execute(cmd, err) < 0) {
                LOG_ERR("execute cmd[%s] error: %s", 
                        cmd.c_str(), err.back().c_str());
                return -1;
        }
        return 0;
}

int AdfsMgr::Remove(const string &firstDataPath)
{
        vector<struct _AdfsInfo> beforeRemoveMpList, afterRemoveMpList;

        if (GetAllDataPath(beforeRemoveMpList) < 0) {
                LOG_ERR("get all mp dataPath error");
                return -1;
        }

        int key = -1;
        for (size_t i = 0; i < beforeRemoveMpList.size(); i++) {
                if (beforeRemoveMpList[i].firstDataPath != firstDataPath) {
                        afterRemoveMpList.push_back(beforeRemoveMpList[i]);
                } else {
                        key = i;
                }
        }
        if (-1 == key) {
                LOG_ERR("cannot find removable mp");
                return -2;
        }

        // 删除掉指定记录后，剩下的其他记录仍写回配置文件
        if (SetAllDataPath(afterRemoveMpList) < 0) {
                LOG_ERR("set all mp dataPath error");
                return -3;
        }

        // 删除数据库中adfs挂载点的记录
        MysqlClient* mysql = NULL;
        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -4;
        }
        string sql = SQL_DELETE_ADFS_MP(firstDataPath);
        if (!mysql->Query(sql)) {
                LOG_ERR("execute sql: %s error", sql.c_str());
                delete mysql;
                return -5;
        }
        delete mysql;

        // 删除各种路径
        vector<string> err;
        string rmStr = beforeRemoveMpList[key].firstDataPath + " ";
        for (size_t i = 0; i < beforeRemoveMpList[key].otherDataPath.size(); i++) {
                rmStr += beforeRemoveMpList[key].otherDataPath[i] + " ";
        }
        if (Execute(CL_DEL_PATH(rmStr), err) < 0) {
                LOG_ERR("execute cmd[%s] error %s", 
                        CL_DEL_PATH(rmStr).c_str(), err.back().c_str());
                return -6;
        }

        return 0;
}

string AdfsMgr::GetMpStatus(const string &mp)
{
        vector<string> err;
        string cmd = CL_IS_ADFS_MOUNTED(mp);
        if (Execute(cmd, err) < 0) {
                LOG_ERR("execute cmd[%s] error: %s", 
                        cmd.c_str(), err.back().c_str());
                return MP_STATE_UNKNOWN;
        }

        if (err.size() == 1 && err.back() == "mounted") {
                return MP_STATE_MOUNTED;
        }
        return MP_STATE_UMOUNTED;
}

int AdfsMgr::GetAllDataPath(vector<struct _AdfsInfo> &mpList)
{
        string key, value;
        IniConfig ini(DEFAULT_ADFS_CONF);

        mpList.clear();
        if (ini.Open() != 0) {
                LOG_ERR("open adfs config %s error", DEFAULT_ADFS_CONF);
                return -1;
        }
        for (size_t i = 1; ; i++) {
                struct _AdfsInfo tmpInfo;
                vector<string> splitValue;
                key = string("dataPath") + IntToString(i);

                if (ini.GetVal("Data", key, value) < 0) {
                        break;
                }
                Split(value.c_str(), splitValue, ':');
                // LOG_DBG("%s->%s", key.c_str(), value.c_str());
                // for (size_t j = 0; j < splitValue.size(); ++j) {
                //         LOG_DBG("%s", splitValue[j].c_str());
                // }

                tmpInfo.firstDataPath = splitValue.front(); 
                for (size_t j = 1; j < splitValue.size(); ++j) {
                        tmpInfo.otherDataPath.push_back(splitValue[j]);
                }
                mpList.push_back(tmpInfo);
        }
        if (ini.Close() != 0) {
                LOG_ERR("close adfs config %s error", DEFAULT_ADFS_CONF);
                return -2;
        }

        return 0;
}

int AdfsMgr::SetAllDataPath(const vector<struct _AdfsInfo> &mpList)
{
        string key, value;
        vector<string> existPath;
        IniConfig ini(DEFAULT_ADFS_CONF);

        if (ini.Open() != 0) {
                LOG_ERR("open adfs config %s error", DEFAULT_ADFS_CONF);
                return -1;
        }

        // 删除原有的路径
        for (size_t i = 0; ; i++) {
                key = "dataPath" + IntToString(i + 1);
                if (ini.GetVal("Data", key, value) == 0) {
                        ini.DelKey("Data", "dataPath" + IntToString(i + 1));
                } else {
                        break;
                }
        }

        // 添加路径
        for (size_t i = 0; i < mpList.size(); i++) {
                key = "dataPath" + IntToString(i + 1);
                value = mpList[i].firstDataPath;
                for (size_t j = 0; j < mpList[i].otherDataPath.size(); ++j) {
                        value += ":" + mpList[i].otherDataPath[j];
                }
                if (ini.SetVal("Data", key, value) < 0) {
                        LOG_ERR("set value: %s to key: %s error", 
                                        value.c_str(), key.c_str());
                        return -2;
                }
        }
        if (ini.Close() != 0) {
                LOG_ERR("close adfs config %s error", DEFAULT_ADFS_CONF);
                return -3;
        }

        return 0;
}

int AdfsMgr::GetDataPathKey(const string &firstDataPath, string &pathKey)
{
        string key, value;
        IniConfig ini(DEFAULT_ADFS_CONF);

        if (ini.Open() != 0) {
                LOG_ERR("open adfs config %s error", DEFAULT_ADFS_CONF);
                return -1;
        }
        for (size_t i = 0; ; i++) {
                key = "dataPath" + IntToString(i + 1);
                if (ini.GetVal("Data", key, value) == 0 && 
                    value.substr(0, value.find(":")) == firstDataPath) {
                        pathKey = key;
                        return 0;
                }
        }
        return -1;
}

bool AdfsMgr::IsPathListValid(const vector<string> &newPathList,
                              const vector<string> &oldPathList)
{
        string cmd;
        vector<string> err;
        vector<string>::const_iterator vsItOld = oldPathList.begin();

        for (; vsItOld != oldPathList.end(); ++vsItOld) {
                vector<string>::const_iterator vsItNew = newPathList.begin();
                for (; vsItNew != newPathList.end(); ++vsItNew) {
                        if (IsSameDev(*vsItNew, *vsItOld)) {
                                LOG_WRN("new path %s has same dev with %s",
                                        vsItNew->c_str(), vsItOld->c_str());
                                return false;
                        }
                }
        }
        return true;
}
