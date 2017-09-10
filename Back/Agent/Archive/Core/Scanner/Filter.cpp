/*******************************************************************************
 *
 * @ File Name  : Filter.cpp
 * @ Date       : 2012-10-17
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 
 * @ History    : 2012-10-17：创建
 *
 * ****************************************************************************/
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "Filter.h"
#include "Utils/Log/Log.h"

/* constructor */
Filter::Filter()
{
}

/* destructor */
Filter::~Filter()
{
}

/**
 * DoFilter() -虚函数，被各过滤子类继承和重写，用于判断文件是否符合要求
 * @path: 被检查文件路径
 * @arg: 过滤条件结构体
 * @return: true 符合条件
 *          false 不符合条件 
 */
bool Filter::DoFilter(const string &path)
{
        struct stat st;

        if (lstat(path.c_str(), &st) < 0) {
                LOG_ERR("stat %s error: %s", path.c_str(), strerror(errno));
                return false;
        }
        if (S_ISREG(st.st_mode)) {
                LOG_DBG("file filter ok: %s\n", path.c_str());
                return true;
        } else {
                LOG_DBG("file filter error: %s\n", path.c_str());
                return false;
        }
}
