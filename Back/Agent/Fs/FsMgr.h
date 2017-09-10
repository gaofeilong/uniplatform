/*******************************************************************************
 *
 * @ File Name  : FsMgr.cpp
 * @ Date       : 2013-10-29
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 文件系统管理父类
 * @ History    : 2013-10-29：创建
 *
 * ****************************************************************************/

#ifndef _FS_MGR_H_
#define _FS_MGR_H_

#include <string>
#include <vector>

using std::vector;
using std::string;

#define MP_STATE_MOUNTED        "mounted"
#define MP_STATE_UMOUNTED       "umounted"
#define MP_STATE_ERROR          "error"
#define MP_STATE_UNKNOWN        "unknown"
#define MP_STATE_READONLY       "readonly"
#define MP_STATE_NOSPACE        "nospace"

class FsMgr {
public:
        FsMgr();
        ~FsMgr();

public:
        virtual string GetMpStatus(const string &mp) = 0;

protected:
        bool InitPath(const string &path);
        bool IsSameDev(const string &dir1, const string &dir2);
};

#endif  //_FS_MGR_H_
