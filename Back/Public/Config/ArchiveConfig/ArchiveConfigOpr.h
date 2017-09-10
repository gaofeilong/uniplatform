#ifndef _Archive_CONFIG_OPR_H_
#define _Archive_CONFIG_OPR_H_

#include <map>
#include <string>
#include <vector>

#include <stdint.h>

#include "Public/Config/ConfigOpr.h"

class MysqlClient;

using std::map;
using std::vector;
using std::string;

struct PathPair {
        PathPair(const string &s, const string &d, const string &b):
                m_Src(s), m_Dest(d), m_Backup(b) {}
        string m_Src;
        string m_Dest;
        string m_Backup;
};

typedef struct _ArchiveState {
        _ArchiveState(const string& i, const string& a, int c=0, const string& ie="0")
        {
                ip            = i;
                archiveState  = a;
                counter       = c;
                isError       = ie;
        }
        int    counter;
        string ip;
        string archiveState;
        string isError;
} ArchiveState;

typedef struct _ArchiveConfig {
        _ArchiveConfig()
        {
                duration        = -1;
                interval        = -1;  
                modifyTime      = -1;  
                isCheck         = -1; 
                isDeleteSource  = -1; 
                prefixType      = 0;  
                postfixType     = 0;  
                dirType         = 0;  
                sizeType        = 0;  
                minSize         = -1;
                maxSize         = -1;
        }
        string  name;                           // 归档名称
        string  startTime;                      // 启动时间
        int     id;                             // 归档ID
        int     duration;                       // 运行时长
        int     interval;                       // 启动间隔
        int     modifyTime;                     // 归档数据时间
        int     isCheck;                        // 是否校验
        int     isDeleteSource;                 // 完成后是否删除源数据

        vector<struct PathPair> pathPair;       // 归档源目标路径对
        map<string, ArchiveState> arvStateSet;  // 归档任务包含IP列表
                                
        int     prefixType;                     // 文件最大size/0:不设置；1:指定；2:过滤
        string  prefix;                         // 前缀格式
        int     postfixType;                    // 0:不设置；1:指定；2:过滤
        string  postfix;                        // 后缀格式
        int     dirType;                        // 不设置；1:指定；2:过滤
        string  dir;                            // 全路径格式
        int     sizeType;                       // 不设置；1:指定；2:过滤
        int64_t minSize;                        // 文件最小size
        int64_t maxSize;                        // 文件最大size
} ArchiveConfig;


class ArchiveConfigOpr : public ConfigOpr<struct _ArchiveConfig> {
public:
        ArchiveConfigOpr(const string& configPath);
        ~ArchiveConfigOpr();

public:
        int Read(struct _ArchiveConfig& ddfsConfig);
        int Write(const struct _ArchiveConfig& ddfsConfig);

private:
        int Exist2Delete(MysqlClient* mysql, const string& tableName, 
                         const string& idName, const string& idVal);
};

#endif //_Archive_CONFIG_OPR_H_
