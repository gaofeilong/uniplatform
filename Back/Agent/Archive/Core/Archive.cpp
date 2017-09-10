#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Archive.h"
#include "Include/Types.h"
#include "Include/String.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Common/DirFileOpr.h"
#include "Utils/Common/CommonFunc.h"
#include "Public/Email/Email.h"
#include "Public/Config/ConfigReader.h"

using std::vector;
using std::make_pair;
using namespace boost::posix_time;

Archive::Archive(const string &id, const string &ip, const string &nodeId)
{
        m_Timer  = time(NULL);
        m_Id     = id;
        m_Ip     = ip;
        m_NodeId = nodeId;
}

Archive::~Archive()
{
}

int Archive::Init()
{
        int ret;
        ConfigReader<ArchiveConfig> cr(m_Id);

        ret = cr.Read(m_Config);
        if (ret < 0) {
                LOG_ERR("read archive config error: %s", m_Id.c_str());
                return -1;
        }

        LOG_DBG("m_Config.name: %s", m_Config.name.c_str());
        LOG_DBG("m_Config.startTime: %s", m_Config.startTime.c_str());
        LOG_DBG("m_Config.duration: %d", m_Config.duration);
        LOG_DBG("m_Config.interval: %d", m_Config.interval);
        LOG_DBG("m_Config.modifyTime: %d", m_Config.modifyTime);
        LOG_DBG("m_Config.isCheck: %d", m_Config.isCheck);
        LOG_DBG("m_Config.isDeleteSource: %d", m_Config.isDeleteSource);
        LOG_DBG("m_Config.prefixType: %d", m_Config.prefixType);
        LOG_DBG("m_Config.prefix: %s", m_Config.prefix.c_str());
        LOG_DBG("m_Config.postfixType: %d", m_Config.postfixType);
        LOG_DBG("m_Config.postfix: %s", m_Config.postfix.c_str());
        LOG_DBG("m_Config.dirType: %d", m_Config.dirType);
        LOG_DBG("m_Config.dir: %s", m_Config.dir.c_str());
        LOG_DBG("m_Config.sizeType: %d", m_Config.sizeType);
        LOG_DBG("m_Config.minSize: %d", m_Config.minSize);
        LOG_DBG("m_Config.maxSize: %d", m_Config.maxSize);
        vector<struct PathPair>::iterator vpIt = m_Config.pathPair.begin();
        for (; vpIt != m_Config.pathPair.end(); ++vpIt) {
                LOG_DBG("%s==arv==>%s==backup==>%s", vpIt->m_Src.c_str(),
                                vpIt->m_Dest.c_str(),vpIt->m_Backup.c_str());
        }

        return 0;
}

/* 根据cron条件而定，有可能不调用DoStart */
int Archive::CronStart()
{
        int ret;
        MysqlData md;
        MysqlClient* mysql = NULL;
        LOG_DBG("archive started in crond");

        ret = Init();
        if (ret < 0) {
                LOG_ERR("initialize archive task error");
                return -1;
        }

        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB error!\n");
                return -1;
        }

        // 正常情况下，数据库中 counter 初始化为启动间隔，没运行 CronStart
        // 一次会减1，等于当减1后等于0时运行归档
        string sql;
        map<string, ArchiveState>::iterator msasIter = m_Config.arvStateSet.find(m_Ip);
        if (msasIter == m_Config.arvStateSet.end()) {
                LOG_ERR("don't have this ip archive.");
                return -1;
        }
        int cnt = (msasIter->second).counter;

        if (cnt > 0) {
                sql = "update node_archive_state set counter=" + IntToString(cnt-1) +
                      " where archive_id=" + m_Id + " and data_node_id=" + m_NodeId;

                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        delete mysql;
                        return -1;
                }
                LOG_DBG("update mysql-counter to %d and quit archive", cnt - 1);
                delete mysql;
        } else {
                sql = "update node_archive_state set counter = " + 
                      IntToString(m_Config.interval - 1) + " where archive_id=" + m_Id + 
                      " and data_node_id=" + m_NodeId;
                if (!mysql->Query(sql)) {
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        delete mysql;
                        return -1;
                }
                delete mysql;

                ret = DoStart();
                if (ret < 0) {
                        LOG_ERR("start archie error");
                        return -1;
                }
        }

        return 0;
}

int Archive::ManualStart()
{
        int ret;
        LOG_DBG("manual start archive\n");

        ret = Init();
        if (ret < 0) {
                LOG_ERR("initialize archive task error");
                return -1;
        }
        ret = DoStart();
        if (ret < 0) {
                LOG_ERR("start archie error");
                return -2;
        }

        return 0;
}

int Archive::DoStart()
{
        LOG_DBG("start archive\n");

        int ret;
        string errInfo;
        DirFileOpr dfOpr;
        vector<struct ArvTaskInfo> arvTask;
        vector<struct ArvTaskInfo>::iterator vaIt;

        if (!Scan()) {
                LOG_ERR("scan error");
                return -2;
        }
        ret = ReadArchiveTask(arvTask);
        if (ret < 0) {
                LOG_ERR("get archive task list from database error");
                return -3;
        }

        string mailErr;
        for (vaIt = arvTask.begin(); vaIt != arvTask.end(); ++vaIt) {
                string src  = vaIt->m_Src;
                string dest = vaIt->m_Dest.substr(0, vaIt->m_Dest.length() - src.length());
                if (!Copy(src, dest, vaIt->m_Mtime)) {
                        // email of copy error
                        LOG_ERR("copy [%s --> %s] error", src.c_str(), dest.c_str());
                        mailErr += "\tcopy [" + src + " --> " + dest + "] error\n";
                        if (!dfOpr.IsDirWritable(dest, errInfo)) {
                                LOG_ERR("cannot write to: %s", dest.c_str());
                                mailErr += "\tcannot write to: " + dest;
                                break;
                        }
                        continue;
                }
                
                if (!Link(src, vaIt->m_Dest, vaIt->m_Mtime)) {
                        LOG_ERR("link %s to %s error", src.c_str(), dest.c_str());
                        // email of link error
                        mailErr += "\tlink [" + src + " --> " + dest + "] error\n";
                        continue;
                }

                // LOG_DBG("elapsed: %ld, duration: %d", 
                //         time(NULL) - m_Timer, m_Config.duration * SEC_PER_HOUR);
                if (time(NULL) - m_Timer >= m_Config.duration * SEC_PER_HOUR) {
                        LOG_INF("archive timeout");
                        break;
                }
        }
        if (m_Config.isDeleteSource && !RemoveBackup()) {
                // email of remove error
                mailErr += "\tremove backup data error\n";
                LOG_ERR("remove backup data error");
                ret = -4;
        }
        // send email
        Mail* mail = GetEmailObj("error");
        if (mail == NULL) {
                LOG_ERR("get mail obj error");
        } else {
                if (!mailErr.empty()) {
                        mail->SendMail("ERROR: archive failed", mailErr);
                }
                delete mail;
        }

        return ret;
}

bool Archive::Scan()
{
        struct _FilterArg filterConf = GetFilterConfig(m_Config);
        Scanner scanner(filterConf);
        map<string, string> arvTask;
        set<string>::iterator ssIt;
        set<string> fileSet;
        int ret;

        vector<struct PathPair>::iterator vpIt = m_Config.pathPair.begin();
        for (; vpIt != m_Config.pathPair.end(); ++vpIt) {
                fileSet.clear();

                ret = scanner.Scan(vpIt->m_Src, fileSet);
                if (ret < 0) {
                        LOG_ERR("scan error");
                        return false;
                }

                for (ssIt = fileSet.begin(); ssIt != fileSet.end(); ++ssIt) {
                        if (ssIt->find(ARCHIVE_BACKUP_SUFFIX + "/") == string::npos) {
                                arvTask.insert(make_pair<string, string>(*ssIt, 
                                        vpIt->m_Dest + "/" + m_Ip + *ssIt));
                        }
                }
                ret = WriteArchiveTask(arvTask);
                if (ret < 0) {
                        LOG_ERR("write archive task error");
                        return false;
                }
        }

        return true;
}

bool Archive::Copy(const string& src, const string& dest, const time_t &mtime)
{
        struct stat st;
        vector<string> errInfo;
        string cmd = (m_Config.isCheck == 0?  "rsync -Ra ": "rsync -Rac ");
        cmd += "'" + src + "' '" + dest + "'";

        // 归档之前检查文件是否正常、是否在扫描后被修改
        LOG_DBG("copy %s=>%s", src.c_str(), dest.c_str());
        if (lstat(src.c_str(), &st) < 0) {
                LOG_ERR("get %s stat error: %s", src.c_str(), strerror(errno));
                if (!UpdateState(src, "error")) {
                        LOG_ERR("update file error state error");
                }
                return false;
        } else if (S_ISLNK(st.st_mode)) {
                LOG_INF("file %s is already a symlink", src.c_str());
                if (!UpdateState(src, "link")) {
                        LOG_ERR("update file error state error");
                }
                return true;
        } else if (st.st_mtime != mtime) {
                LOG_ERR("file %s has been modified since scan", src.c_str());
                if (!UpdateState(src, "error")) {
                        LOG_ERR("update file error state error");
                }
                return false;
        }

        // 归档
        if (Execute(cmd, errInfo) < 0) {
                LOG_ERR("execute %s error: %s", cmd.c_str(), errInfo[0].c_str());
                return false;
        }
        if (!UpdateState(src, "copy")) {
                LOG_ERR("update file state to copy error");
                return false;
        }
        return true;
}

bool Archive::Link(const string &src, const string &dest, const time_t &mtime) 
{
        struct stat st;
        string backupDir, backupFile;

        // 获取归档备份文件的目标位置和它的父目录路径
        vector<struct PathPair>::iterator vpIt = m_Config.pathPair.begin();
        for (; vpIt != m_Config.pathPair.end(); ++vpIt) {
                if (src.find(vpIt->m_Src + "/") != string::npos) {
                        backupFile = vpIt->m_Backup + src;
                        backupDir = backupFile.substr(0, backupFile.rfind("/"));
                        break;
                }
        }
        if (backupDir.empty() || MakeDir(backupDir) != 0) {
                LOG_ERR("create backupFile dir %s error", backupDir.c_str());
                return false;
        }

        // 移动、连接之前检查文件是否正常、是否在归档后被修改
        LOG_DBG("rename %s=>%s", src.c_str(), backupFile.c_str());
        if (lstat(src.c_str(), &st) < 0) {
                LOG_ERR("get %s stat error: %s", src.c_str(), strerror(errno));
                if (!UpdateState(src, "error")) {
                        LOG_ERR("update file error state error");
                }
                return false;
        } else if (S_ISLNK(st.st_mode)) {
                LOG_INF("file %s is already a symlink", src.c_str());
                if (!UpdateState(src, "link")) {
                        LOG_ERR("update file error state error");
                }
                return true;
        } else if (st.st_mtime != mtime) {
                LOG_ERR("file %s has been modified since archive", src.c_str());
                if (!UpdateState(src, "error")) {
                        LOG_ERR("update file error state error");
                }
                return false;
        }

        // 移动和链接，连接失败需要把文件移回去，如果再次失败，报错
        LOG_DBG("symlink L%s=>F%s", src.c_str(), dest.c_str());
        if (rename(src.c_str(), backupFile.c_str()) < 0) {
                LOG_ERR("rename %s to %s error: %s", src.c_str(), 
                                backupFile.c_str(), strerror(errno));
                return false;
        }
        if (symlink(dest.c_str(), src.c_str()) < 0) {
                LOG_ERR("create symbol link %s to file %s error: %s, %s: %s",
                        src.c_str(), dest.c_str(), strerror(errno), 
                        "recover file from backup", backupFile.c_str());
                if (rename(backupFile.c_str(), src.c_str()) < 0) {
                        LOG_ERR("rename backup file %s to %s error: %s", 
                             src.c_str(), backupFile.c_str(), strerror(errno));
                        if (!UpdateState(src, "error")) {
                                LOG_ERR("update file error state error");
                        }
                        return false;
                }
                return true;
        }
        
        /* 移动、链接成功后，修改数据库记录的状态 */
        if (!UpdateState(src, "link")) {
                LOG_ERR("update file state to link error");
                return false;
        }
        return true;
}

bool Archive::Rename(const string &src, const string &dest)
{
        int ret;
        LOG_DBG("rename %s=>%s", src.c_str(), dest.c_str());
        ret = rename(src.c_str(), dest.c_str());
        if (ret < 0) {
                LOG_ERR("rename %s to %s error: %s", 
                        src.c_str(), dest.c_str(), strerror(errno));
                return false;
        }
        return true;
}

bool Archive::Symlink(const string &src, const string &dest)
{
        int ret;
        LOG_DBG("symlink %s=>%s", src.c_str(), dest.c_str());
        ret = symlink(src.c_str(), dest.c_str());
        if (ret < 0) {
                LOG_WRN("create symbol link %s to file %s error: %s", 
                        dest.c_str(), src.c_str(), strerror(errno));
                /* rename back */
                return false;
        }
        return true;
}

bool Archive::RemoveBackup()
{
        int ret;
        string cmd;
        bool result = true;
        vector<string> errInfo;

        vector<struct PathPair>::iterator vpIt = m_Config.pathPair.begin();
        for (; vpIt != m_Config.pathPair.end(); ++vpIt) {
                LOG_DBG("remove backup dir: %s", vpIt->m_Backup.c_str());
                cmd = "rm -rf '" + vpIt->m_Backup + "'";
                ret = Execute(cmd, errInfo);
                if (ret < 0) {
                        LOG_ERR("execute %s error: %s", cmd.c_str(), errInfo[0].c_str());
                        result = false;
                        continue;
                }
        }
        return result;
}

bool Archive::UpdateState(const string &src, const string &state)
{
        MysqlData md;
        MysqlClient* mysql = NULL;
        string sql = "update archive_log set state='" + state + "',arvtime='" +
                        GetCurSec() + "' where src='" + src + "' and config_id='" +
                        m_Id + "' and ip='" + m_Ip + "'";

        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB error!\n");
                return false;
        }
        if (!mysql->Query(sql)) { 
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                delete mysql;
                return false;
        }
        delete mysql;
        return true;
}

/* 记录扫描结果到数据库，如果数据库中已经有该记录，当已存在的记录的状态字段是 
 * scan 时，不做处理；状态是 copy 时，更新状态为 scan（原因是可能两次扫描期间
 * 文件被修改了，那么上次已经完成的 copy 是无效的）；状态是 link 时，新插入记
 * 录（link 说明之前归档流程整个走完，现在文件本应该是符号链接，但是这次又扫
 * 描到了，说明符号链接又被改为文件了，应该重新归档，目标目录中的文件被覆盖
 */
int Archive::WriteArchiveTask(const map<string, string> &task)
{
        int ret;
        string sql;
        struct stat st;
        MysqlData md;
        MysqlClient* mysql = NULL;

        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB error!\n");
                return -1;
        }

        map<string, string>::const_iterator mssIt;
        for (mssIt = task.begin(); mssIt != task.end(); ++mssIt) {
                LOG_DBG("%s->%s", mssIt->first.c_str(), mssIt->second.c_str());
                // 删除无效的扫描记录，记录最新的扫描结果
                sql = string("delete from archive_log where config_id=") + m_Id +
                        " and src='" + mssIt->first + "' and ip='" + m_Ip + "' and state!='linked' and state!='error'";
                if (!mysql->Query(sql)) { 
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        continue;
                }

                if (lstat(mssIt->first.c_str(), &st) < 0) {
                        LOG_ERR("get %s stat error: %s", mssIt->first.c_str(), strerror(errno));
                        continue;
                }
                sql = string("insert into archive_log(`config_id`,`ip`,`arvname`,`arvtime`,`mtime`,`src`,")+
                      string("`dest`,`size`,`state`) values(") + m_Id + ", '" + m_Ip + "', '" + m_Config.name+ "','" +
                             GetCurSec() + "'," + IntToString(st.st_mtime) + ",'" + mssIt->first + "', '" + 
                             mssIt->second + "', " + IntToString(st.st_size) + ", 'scan')";

                if (!mysql->Query(sql)) { 
                        LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                        continue;
                }
        }
        delete mysql;

        return 0;
}

int Archive::ReadArchiveTask(vector<struct ArvTaskInfo> &task)
{
        int ret;
        MysqlData md;
        MysqlClient* mysql = NULL;
        string sql = string("select id, src, dest, mtime, size from archive_log where state = 'scan' and config_id= ") + 
                     m_Id + " and ip='" + m_Ip + "'";

        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB error!\n");
                return -1;
        }
        if (!mysql->Query(sql, md)) { 
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                delete mysql;
                return -2;
        }

        task.clear();
        for (size_t i = 0; i < md.Rows(); i++) {
                MysqlDataRow mdr = md.GetRow(i);
                task.push_back(ArvTaskInfo(mdr["src"], 
                        mdr["dest"], atol(mdr["mtime"].c_str())));
        }
        delete mysql;

        return 0;
}

struct _FilterArg Archive::GetFilterConfig(const ArchiveConfig &conf)
{
        struct _FilterArg filterConf;
        GetTimeArg(m_Config.modifyTime, m_Config.interval, filterConf._TimeArg);
        GetFixArg(m_Config.prefixType, m_Config.prefix, filterConf._PrefixArg);
        GetFixArg(m_Config.postfixType, m_Config.postfix, filterConf._PostfixArg);
        GetDirArg(m_Config.dirType, m_Config.dir, filterConf._DirArg);
        GetSizeArg(m_Config.sizeType, m_Config.maxSize, 
                                m_Config.minSize, filterConf._SizeArg);
        Show(filterConf);
        return filterConf;
}

void Archive::GetTimeArg(int dataTime, int interval, struct _TimeFilterArg &arg)
{
        arg._Type = _DEMAND;
        arg._TimeNow = time(NULL);
        arg._TimeBoundaryNear = dataTime;
        arg._TimeBoundaryFar = dataTime + interval - 1;
}

void Archive::GetSizeArg (int type, size_t max, size_t min, 
                                        struct _SizeFilterArg &arg)
{
        if (type == 0) {
                arg._Type = _NOFILTER;
        } else {
                arg._Type = (type == 1? _DEMAND: _FILTER);
                arg._Min = min;
                arg._Max = max;
        }
}

void Archive::GetDirArg(int type, const string &s, struct _DirFilterArg &arg)
{
        arg._Type = (type == 0? _NOFILTER: type == 1? _DEMAND: _FILTER);
        ParseStr(s, arg._DirSet);
}

void Archive::GetFixArg(int type, const string &s, struct _FixFilterArg &arg)
{
        arg._Type = (type == 0? _NOFILTER: type == 1? _DEMAND: _FILTER);
        ParseStr(s, arg._FixSet);
}

void Archive::ParseStr(const string &s, set<string> &set)
{
        char *token, *savePtr;
        char *str = new char[s.size() + 1];

        set.clear();
        memcpy(str, s.c_str(), s.size());
        str[s.size()] = '\0';

        while (1) {
                token = strtok_r(str, ";", &savePtr);
                if (token == NULL) {
                        break;
                }
                set.insert(token);
                str = NULL;
        }
}

void Archive::Show(const struct _FilterArg &filterArg)
{
        /* prefix filter */
        enum _FilterType type = filterArg._PrefixArg._Type;
        if (type == _NOFILTER) {
                LOG_DBG("no prefix filter\n"); 
        } else {
                LOG_DBG("%s prefix :\n", type == _FILTER? "filter": "demand");
                set<string>::iterator it = filterArg._PrefixArg._FixSet.begin();
                for ( ;it != filterArg._PrefixArg._FixSet.end(); ++it) {
                        LOG_DBG("\t%s\n", it->c_str()); 
                }
        }

        /* postfix filter */
        type = filterArg._PostfixArg._Type;
        if (type == _NOFILTER) {
                LOG_DBG("no postfix filter\n"); 
        } else {
                LOG_DBG("%s postfix :\n", type == _FILTER? "filter": "demand");
                set<string>::iterator it = filterArg._PostfixArg._FixSet.begin();
                for ( ;it != filterArg._PostfixArg._FixSet.end(); ++it) {
                        LOG_DBG("\t%s\n", it->c_str()); 
                }
        }

        /* dir filter */
        type = filterArg._DirArg._Type;
        if (type == _NOFILTER) {
                LOG_DBG("no dir filter\n"); 
        } else {
                LOG_DBG("%s dir :\n", type == _FILTER? "filter": "demand");
                set<string>::iterator it = filterArg._DirArg._DirSet.begin();
                for ( ;it != filterArg._DirArg._DirSet.end(); ++it) {
                        LOG_DBG("\t%s\n", it->c_str()); 
                }
        }

        /* size filter */
        type = filterArg._SizeArg._Type;
        if (type == _NOFILTER) {
                LOG_DBG("no size filter\n"); 
        } else {
                LOG_DBG("%s size:\n", type == _FILTER? "filter": "demand");
                LOG_DBG("\tmin: %ld\n", filterArg._SizeArg._Min);
                LOG_DBG("\tmax: %ld\n", filterArg._SizeArg._Max);
        }

        /* time filter */
        type = filterArg._TimeArg._Type;
        if (type == _NOFILTER) {
                LOG_DBG("no time filter\n"); 
        } else {
                LOG_DBG("%s time:\n", type == _FILTER? "filter": "demand");
                LOG_DBG("\tnow: %ld\n", filterArg._TimeArg._TimeNow);
                LOG_DBG("\tboundery far: %ld\n", filterArg._TimeArg._TimeBoundaryFar);
                LOG_DBG("\tboundery near: %ld\n", filterArg._TimeArg._TimeBoundaryNear);
        }
}
