/*******************************************************************************
 *
 * @ File Name  : SizeFilter.cpp
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

#include "SizeFilter.h"
#include "Utils/Log/Log.h"

/* constructor */
SizeFilter::SizeFilter(BOOSTPTR<Filter> &ptr, 
                        const struct _SizeFilterArg &arg): 
        m_FilterPtr(ptr), m_SizeArg(arg)
{
}

/* destructor */
SizeFilter::~SizeFilter()
{
}

/**
 * DoFilter() -归档文件大小过滤函数，继承自 Filter 类
 * @path: 被检查文件路径
 * @arg: 过滤条件结构体
 * @return: true 符合条件
 *          false 不符合条件 
 */
bool SizeFilter::DoFilter(const string &path)
{
        struct stat st;

        if (lstat(path.c_str(), &st) < 0) {
                LOG_ERR("stat %s error: %s", path.c_str(), strerror(errno));
                return false;
        }
        if (m_SizeArg._Type == _DEMAND) {                 /* 指定 */
                if (!InRange(st.st_size)) {                     /* 不符合被指定 */
                        LOG_DBG("size filter error: %s\n", path.c_str());
                        return false;
                }
        } else {
                if(InRange(st.st_size)) {                       /* 符合被过滤 */
                        LOG_DBG("size filter error: %s\n", path.c_str());
                        return false;
                }
        }
        //LOG_DBG("size filter ok: %s\n", path.c_str());
        
        /* 属于被指定的或者没有被过滤掉，进入下一次过滤条件判断 */
        LOG_DBG("size filter ok: %s\n", path.c_str());
        return m_FilterPtr->DoFilter(path);
}

/**
 * InRange() -判断文件大小是否符合要求
 * @size: 被检查文件大小
 * @sizeArg: 文件大小过滤条件结构体
 * @return: true 符合条件
 *          false 不符合条件 
 */
bool SizeFilter::InRange(size_t size)
{
        if (m_SizeArg._Min == 0) {                /* 小于某个值 */
                return size <= m_SizeArg._Max;
        } else if (m_SizeArg._Max == 0) {         /* 大于某个值 */
                return size >= m_SizeArg._Min;
        } else {                                /* 介于某两个值之间 */
                return size <= m_SizeArg._Max && size >= m_SizeArg._Min;
        }
}
