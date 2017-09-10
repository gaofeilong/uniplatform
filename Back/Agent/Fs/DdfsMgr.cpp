/* 
 * @ Date        : 2012-09-25
 * @ Auther      : wangbo
 * @ Description : 对ddfs单机文件系统的管理
 * @ Email       : wang.bo@scidata.cn
 * @ History     : 2012-09-04 创建
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <algorithm>

#include "DdfsMgr.h"
#include "Include/Types.h"
#include "Include/String.h"
#include "Public/Mysql/Mysql.h"
#include "Public/Config/ConfigReader.h"
#include "Utils/Log/Log.h"
#include "Utils/Locker/Locker.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Common/DirFileOpr.h"
#include "Utils/Common/CommonFunc.h"

DdfsMgr::DdfsMgr()
{ 
}

DdfsMgr::~DdfsMgr() 
{
}

int DdfsMgr::GetConfigByMp(const string& mp, string& conf)
{
        MysqlData md;
        MysqlClient* db;
        string sqlLine = "select config from ddfsmp where mp='" + mp + "'";

        if (GetDB(&db) < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }
        if (!db->Query(sqlLine, md)) {
                LOG_ERR("Execute Error! sql=%s\n", sqlLine.c_str());
                delete db;
                return -2;
        }
        conf = md.GetData(0 ,0);
        delete db;

        return 0;
}

int DdfsMgr::GetMpList(vector<struct _DdfsPair>& mpList)
{
        MysqlClient* db = NULL;
        int ret = GetDB(&db);
        if (ret < 0) {
                LOG_ERR("GetDB Error!");
                return -1;
        }

        MysqlData md;
        string sqlLine = "select id,mp,config from ddfsmp";
        if (!db->Query(sqlLine, md)) {
                delete db;
                LOG_ERR("Execute Error! sql=%s\n", sqlLine.c_str());
                return -1;
        }

        /* 存放一行的信息 */
        MysqlDataRow mdr;
        /* 返回表中信息总共有多少行 */
        size_t row = md.Rows();
       
        for (size_t i=0; i<row; ++i) {
                mdr = md.GetRow(i);
                struct _DdfsPair ddfsPair;
                ddfsPair.id     = mdr[FIELD_ID];
                ddfsPair.mp     = mdr[FIELD_MP];
                ddfsPair.config = mdr[FIELD_CONFIG];
                mpList.push_back(ddfsPair);
        }

        delete db;
        return 0;
}

string DdfsMgr::GetMpStatus(const string &mp)
{
        int ret = 0;
        DirFileOpr dfOpr;
        string errInfo, conf;
        vector<string> tmpVec;

        if (GetConfigByMp(mp, conf) < 0) {
                LOG_ERR("get config by mp: %s error", mp.c_str());
                return MP_STATE_UNKNOWN;
        }

        string cmd = CL_GET_FIRST_DATA_PATH(conf);
        ret = Execute(cmd, tmpVec);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s\n", cmd.c_str());
                return MP_STATE_UNKNOWN;
        }
        // 配置文件不存在或非法配置文件
        if (tmpVec.empty() || tmpVec.size() > 2) {
                return MP_STATE_UNKNOWN;
        }

        // 通过加锁来判断ddfs挂载点状态
        string ddfsPidFile = CL_GET_DDFS_PID(tmpVec[0]);
        if (HasPath(ddfsPidFile)) {    
                Locker lock;
                ret = lock.TryLockFile(ddfsPidFile);
                if (ret < 0) {
                        LOG_ERR("TryLockFile() Error! pid=%s\n", ddfsPidFile.c_str());
                        return MP_STATE_UNKNOWN;
                } else if (ret == LOCK_FAIL) {
                        return dfOpr.IsDirWritable(mp, errInfo)? 
                                MP_STATE_MOUNTED: MP_STATE_READONLY;
                }
        } else {
                return MP_STATE_UMOUNTED;
        }
        return MP_STATE_ERROR;
}

int DdfsMgr::GetDedupInfo(const string& mp, struct _DedupInfo &dedupInfo)
{
        int ret = 0;
        string cmd = CL_DDFS_DEDUP(mp);
        vector<string> vec;
        ret = Execute(cmd, vec);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s\n", cmd.c_str());
                return -1;
        }
        if (vec.size() == 3) {
                dedupInfo.total = vec[0];
                dedupInfo.real  = vec[1];
                dedupInfo.ratio = vec[2];
        } else {
                LOG_ERR("get %s dedup info error", mp.c_str());
        }
        return 0;
}

int DdfsMgr::Create(const struct _DdfsConfig& ddfsConfig, vector<string>& errVec)
{
        int ret = 0;
        DirFileOpr dfOpr;
        vector<string> err;
        string errInfo, allNewPath;

        // 路径检查 
        vector<string>::const_iterator vsIt = ddfsConfig.dataPathList.begin();
        for (; vsIt != ddfsConfig.dataPathList.end(); ++vsIt) {
                if (!InitPath(*vsIt)) {
                        errInfo = string("init dataPath error: ") + *vsIt;
                        LOG_ERR(errInfo.c_str());
                        errVec.push_back(errInfo);
                        if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                                LOG_ERR("clear new path %s error", allNewPath.c_str());
                        }
                        return -5;
                }
                allNewPath += *vsIt + " ";
        }
        if (!InitPath(ddfsConfig.mountPoint)) {
                errInfo = string("init mountpoint error: ") + ddfsConfig.mountPoint;
                LOG_ERR(errInfo.c_str());
                errVec.push_back(errInfo);
                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                        LOG_ERR("clear new path %s error", allNewPath.c_str());
                }
                return -1;
        }
        allNewPath += ddfsConfig.mountPoint + " ";
        if (!InitPath(ddfsConfig.biPath)) {
                errInfo = string("init biPath error: ") + ddfsConfig.biPath;
                LOG_ERR(errInfo.c_str());
                errVec.push_back(errInfo);
                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                        LOG_ERR("clear new path %s error", allNewPath.c_str());
                }
                return -2;
        }
        allNewPath += ddfsConfig.biPath+ " ";
        if (!InitPath(ddfsConfig.metaPath)) {
                errInfo = string("init metaPath error: ") + ddfsConfig.metaPath;
                LOG_ERR(errInfo.c_str());
                errVec.push_back(errInfo);
                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                        LOG_ERR("clear new path %s error", allNewPath.c_str());
                }
                return -3;
        }
        allNewPath += ddfsConfig.metaPath+ " ";
        if (!InitPath(ddfsConfig.mapPath)) {
                errInfo = string("init mapPath error: ") + ddfsConfig.mapPath;
                LOG_ERR(errInfo.c_str());
                errVec.push_back(errInfo);
                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                        LOG_ERR("clear new path %s error", allNewPath.c_str());
                }
                return -4;
        }
        allNewPath += ddfsConfig.mapPath+ " ";
        
        /* 拷贝默认配置文件到指定的路径下 */
        string cmd = CL_CP(DDFS_DEF_CONF_FILE, ddfsConfig.dataPathList[0]);
        ret = Execute(cmd, errVec);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s\n", cmd.c_str());
                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                        LOG_ERR("clear new path %s error", allNewPath.c_str());
                }
                return ret;
        }

        /* 修改默认配置文件中对应字段 */
        string configPath = CL_GET_DDFS_CONF(ddfsConfig.dataPathList[0]);
        ConfigReader<struct _DdfsConfig> ddfsCr(configPath);
        ret = ddfsCr.Write(ddfsConfig);
        if (ret < 0) {
                LOG_ERR("Write Error! config=%s\n", configPath.c_str());
                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                        LOG_ERR("clear new path %s error", allNewPath.c_str());
                }
                return ret;
        }

        /* 2. 执行初始化操作 */
        ret = Mkfs(configPath, errVec);
        if (ret < 0) {
                LOG_ERR("Mkfs Error! config=%s\n", configPath.c_str());
                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                        LOG_ERR("clear new path %s error", allNewPath.c_str());
                }
                return ret;
        }

        /* 3. 保存到数据库中 */
        MysqlClient* db = NULL;
        ret = GetDB(&db);
        if (ret < 0) {
                LOG_ERR("GetDB Error!\n");
                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                        LOG_ERR("clear new path %s error", allNewPath.c_str());
                }
                return -1;
        }

        string sqlLine = "insert into ddfsmp(`mp`, `config`) values ('" + \
                          ddfsConfig.mountPoint + "', '" + configPath + "')";
        if (!db->Query(sqlLine)) {
                LOG_ERR("Execute Error! sql=%s\n", sqlLine.c_str());
                if (Execute(CL_DEL_PATH(allNewPath), err) < 0) {
                        LOG_ERR("clear new path %s error", allNewPath.c_str());
                }
        }
        delete db;
        return ret;
}

int DdfsMgr::Mkfs(const string& configPath, vector<string>& errVec)
{
        int ret = 0;
        string cmd = CL_DDFS_MKFS(configPath);
        ret = Execute(cmd, errVec);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s\n", cmd.c_str());
                ret = -1;
        }
        return ret;
}

int DdfsMgr::Mount(const struct _DdfsPair& ddfsPair, vector<string>& errVec)
{
        string conf;
        /* 1. 执行 ddfs_mount */
        int ret = 0;
        string cmd = CL_DDFS_MOUNT(ddfsPair.mp, ddfsPair.config);
        ret = Execute(cmd, errVec);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s\n", cmd.c_str());
                ret = -1;
        }
        return ret;
}

int DdfsMgr::Umount(const struct _DdfsPair& ddfsPair, vector<string>& errVec)
{
        int ret = 0;
        /* 判断归档运行情况 */
        ret = GetArchiveStateByDest(ddfsPair.mp);
        if (ret < 0) {
                LOG_ERR("GetArchiveStateByDest Error! mp=%s\n", ddfsPair.mp.c_str());
                errVec.push_back("get archive state error");
                return ret;
        }

        if (ret > 0) {
                LOG_INF("archive is running\n");
                errVec.push_back("archive is running, can't umount this mountpoint");
                return -1;
        }

        /* 1. 执行 ddfs_umount */
        string conf;
        string cmd = CL_DDFS_UMOUNT(ddfsPair.mp, ddfsPair.config);
        ret = Execute(cmd, errVec);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s\n", cmd.c_str());
                ret = -1;
        }
        return ret;
}

int DdfsMgr::Fsck(const string& mountPoint, const string& configPath, vector<string>& errVec)
{
        /* 1. 执行 fsck.ddfs */
        int ret = 0;
        DirFileOpr dfOpr;
        if (dfOpr.IsMounted(DDFS_FILESYSTEM, mountPoint)) {
                ret = ::Umount(mountPoint);
                if (ret != 0) {
                        errVec[0] = "umount " + mountPoint + " error";
                        return -1;
                }
        }

        string cmd = CL_DDFS_FCSK(configPath);
        ret = Execute(cmd, errVec);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s\n", cmd.c_str());
                ret = -1;
        }
        return ret;
}

int DdfsMgr::Modify(const string& configPath, const vector<string> allPathList,
                        vector<string>& errVec)
{
        int ret = 0;

        // 获取ddfs配置文件内容
        struct _DdfsConfig ddfsConfig;
        ret = LookConf(configPath, ddfsConfig);
        if (ret < 0) {
                LOG_ERR("LookConf Error!\n");
                errVec.push_back("get ddfs config info error");
                return ret;
        }

        bool isMake = false;
        vector<string> addList;
        vector<string> &curPathList = ddfsConfig.dataPathList;

        for (size_t i = 0; i < allPathList.size(); ++i) {
                // 把原有的路径过滤掉
                if (std::find(curPathList.begin(), curPathList.end(),
                    allPathList[i]) != curPathList.end()) {
                        continue;
                }

                // 判断是否和已有路径挂载设备相同
                for (size_t j = 0; j != curPathList.size(); ++j) {
                        // 创建不存在的新目录
                        if (!HasPath(allPathList[i])) {
                                if (MakeDir(allPathList[i]) < 0) {
                                        LOG_ERR("create dir error! path=%s\n", allPathList[i].c_str());
                                        errVec.push_back("create [" + allPathList[i] + "] error");
                                        return -1;
                                }
                                isMake = true;
                        }

                        string cmd = CL_DIFF_DEVICE(curPathList[j], allPathList[i]);
                        ret = Execute(cmd, errVec);
                        if (ret != 0) {
                                LOG_ERR("Execute error!\n");
                                if (isMake) {
                                        rmdir(allPathList[i].c_str());
                                }
                                errVec.clear();
                                errVec.push_back("get deveice error: " + curPathList[j] + 
                                                 " ," + allPathList[i]);
                                return ret;
                        }

                        if (errVec.size() == 1) {
                                if (isMake) {
                                        rmdir(allPathList[i].c_str());
                                }
                                errVec.clear();
                                errVec.push_back("dataPath has same st_dev: " + 
                                                   curPathList[j] + ", " + allPathList[i]);
                                return -1;
                        }
                }
                addList.push_back(allPathList[i]);

                isMake = false;
        }
        ddfsConfig.dataPathList = allPathList;

        // 如果是挂载状态，使用动态添加路径的工具添加数据路径
        ConfigReader<struct _DdfsConfig> ddfsCr(configPath);
        ret = ddfsCr.Write(ddfsConfig);
        if (ret < 0) {
                LOG_ERR("Write Error! config=%s\n", configPath.c_str());
                errVec.push_back("edit ddfs config error");
                return -2;
        }

        string state = GetMpStatus(ddfsConfig.mountPoint);
        if (state == MP_STATE_MOUNTED) {
                for (size_t i = 0; i != addList.size(); ++i) {
                        string cmd = "util.ddfs -vol " + 
                                      addList[i] + " " + configPath;
                        ret = Execute(cmd, errVec);
                        if (ret < 0) {
                                LOG_ERR("Execute error! %s\n", cmd.c_str());
                                return -3;
                        }
                }
        }
        return 0;
} 
int DdfsMgr::LookConf(const string& configPath, struct _DdfsConfig& ddfsConfig)
{
        int ret = 0;
        ConfigReader<struct _DdfsConfig> ddfsCr(configPath);
        ret = ddfsCr.Read(ddfsConfig);
        if (ret < 0) {
                LOG_ERR("Write Error! config=%s\n", configPath.c_str());
        }
        return ret;
}

int DdfsMgr::Remove(const string& configPath)
{
        int ret = 0;
        struct _DdfsConfig ddfsConfig;
        ConfigReader<struct _DdfsConfig> ddfsCr(configPath);
        ret = ddfsCr.Read(ddfsConfig);
        if (ret < 0) {
                LOG_ERR("Write Error! config=%s\n", configPath.c_str());
                return ret;
        }

        string rmStr = ddfsConfig.biPath + " " + ddfsConfig.mapPath + " " + 
                       ddfsConfig.metaPath + " " + ddfsConfig.mountPoint + " ";
        for (size_t i=0; i<ddfsConfig.dataPathList.size(); ++i) {
                rmStr += ddfsConfig.dataPathList[i] + " ";
        }

        vector<string> errVec;
        ret = Execute(CL_DEL_PATH(rmStr), errVec);
        if (ret < 0) {
                LOG_ERR("Execute Error! cmd=%s\n", CL_DEL_PATH(rmStr).c_str());
        }
        /* 删除数据库内容 */
        MysqlClient* db = NULL;
        ret = GetDB(&db);
        if (ret < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }
        string sqlLine = "delete from ddfsmp where config='" + configPath + "'";
        if (!db->Query(sqlLine)) {
                LOG_ERR("Execute Error! sql=%s\n", sqlLine.c_str());
        }
        delete db;
        return ret;
}

int DdfsMgr::GetNodeIdByIp(const string& ip, string& id)
{
        MysqlClient* db = NULL;
        int ret = GetDB(&db);
        if (ret < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }

        string where = FIELD_IP + string("='") + ip + "'";

        MysqlData md;
        /* 拼装sql语句 */
        string sqlLine = SELECT_WHERE(TABLE_NODE_INFO, FIELD_ID, where);
        if (!db->Query(sqlLine, md)) {
                delete db;
                LOG_ERR("Execute Error! sql=%s\n", sqlLine.c_str());
                return -1;
        }

        /* 获取当前节点的ID */
        id = md.GetData(0, "node_id");

        delete db;
        return 0;
}

int DdfsMgr::GetArchiveStateByDest(const string& mp)
{
        MysqlClient* db = NULL;
        int ret = GetDB(&db);
        if (ret < 0) {
                LOG_ERR("GetDB Error!\n");
                return -1;
        }

        /* 拼装sql语句 */
        string sqlLine = "select config_id from archive_path_pair \
                          where dest_path='" + mp + "'";
        MysqlData md;
        if (!db->Query(sqlLine, md)) {
                delete db;
                LOG_ERR("Execute Error! sql=%s\n", sqlLine.c_str());
                return -1;
        }

        bool state = false;
        int row = md.Rows();
        for (int i=0; i<row; ++i) {
                MysqlDataRow mdr = md.GetRow(i);
                string arvCmd = ARCHIVE_MAIN + " " + ARCHIVE_CONF(mdr["config_id"]);
                ret = IsProRunning(arvCmd, state);
                if (ret < 0) {
                        delete db;
                        LOG_ERR("IsProRunning Error! cmd=%s\n", arvCmd.c_str());
                        return -2;
                }
                if (state) {
                        delete db;
                        return 1;
                }
        }

        delete db;
        return 0;
}
