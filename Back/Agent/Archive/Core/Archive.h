/*************************************************
 * Archiveright (C), 2010-2011, Scidata Tech. Co., Ltd.
 *
 * Description: 拷贝文件，校验文件MD5
 * 
 * Author: gfl
 **************************************************/
#ifndef _ARCHIVE_H_
#define _ARCHIVE_H_

#include <string>

#include "Scanner/Scanner.h"
#include "Public/Mysql/Mysql.h"
#include "Public/Config/ArchiveConfig/ArchiveConfigOpr.h"

using std::string;

struct ArvTaskInfo {
        ArvTaskInfo(const string &src, const string &dest, const time_t t):
                m_Src(src), m_Dest(dest), m_Mtime(t) {}
        string m_Src;
        string m_Dest;
        time_t m_Mtime;
};

class Archive
{
public:
        Archive(const string &id, const string &ip, const string &nodeId);
        ~Archive();
public:
        int CronStart();
        int ManualStart();

private:
        int Init();
        int DoStart();

        bool Scan();
        bool RemoveBackup();
        bool UpdateState(const string &src, const string &state);
        bool Copy(const string &src, const string &dest, const time_t &mtime);
        bool Link(const string &src, const string &dest, const time_t &mtime);
        bool Rename(const string &src, const string &dest);
        bool Symlink(const string &src, const string &link);

        int ReadArchiveTask(vector<struct ArvTaskInfo> &task);
        int WriteArchiveTask(const map<string, string> &task);

        /* 辅助函数，用于构造过滤结构体 */
        struct _FilterArg GetFilterConfig(const ArchiveConfig &conf);
        void GetTimeArg(int dataTime, int interval, struct _TimeFilterArg &arg);
        void GetFixArg(int type, const string &s, struct _FixFilterArg &arg);
        void GetDirArg(int type, const string &s, struct _DirFilterArg &arg); 
        void GetSizeArg (int type, size_t max, size_t min, struct _SizeFilterArg &arg);
        void ParseStr(const string &s, set<string> &set);
        void Show(const struct _FilterArg &filterArg);

private:
        time_t m_Timer;
        string m_Id;
        string m_Ip;
        string m_NodeId;
        ArchiveConfig m_Config;
};

#endif
