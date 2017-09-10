/*******************************************************************************
 *
 * @ File Name  : FsMgr.cpp
 * @ Date       : 2013-10-29
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 文件系统管理父类
 * @ History    : 2013-10-29：创建
 *
 * ****************************************************************************/

#include "FsMgr.h"
#include "Include/String.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Common/DirFileOpr.h"

FsMgr::FsMgr()
{ 
}

FsMgr::~FsMgr() 
{
}

bool FsMgr::InitPath(const string &path)
{
        DirFileOpr dfOpr;
        if (dfOpr.HasPath(path)) {
                if (dfOpr.IsDir(path)) {
                        if (!dfOpr.IsDirEmpty(path)) {
                                LOG_ERR("%s is not a empty directory", path.c_str());
                                return false;
                        }
                } else {
                        LOG_ERR("%s not a directory", path.c_str());
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

bool FsMgr::IsSameDev(const string &dir1, const string &dir2)
{
        vector<string> err;
        string cmd = CL_DIFF_DEVICE(dir1, dir2);
        if (Execute(cmd, err) < 0) {
                LOG_ERR("execute cmd[%s] error: %s", 
                        cmd.c_str(), err.back().c_str());
                return false;
        }
        return 1 == err.size()? true: false;
}
