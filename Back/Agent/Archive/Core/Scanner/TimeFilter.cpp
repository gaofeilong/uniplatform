/*******************************************************************************
 *
 * @ File Name  : TimeFilter.cpp
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
#include <boost/date_time/gregorian/gregorian.hpp>

#include "TimeFilter.h"
#include "Utils/Log/Log.h"

using namespace boost::gregorian;

/* constructor */
TimeFilter::TimeFilter(BOOSTPTR<Filter> &ptr, 
                        const struct _TimeFilterArg &arg): 
        m_FilterPtr(ptr), m_TimeArg(arg)
{
}

/* destructor */
TimeFilter::~TimeFilter()
{
}

/**
 * DoFilter() -归档文件大小过滤函数，继承自 Filter 类
 * @path: 被检查文件路径
 * @arg: 过滤条件结构体
 * @return: true 符合条件
 *          false 不符合条件 
 */
bool TimeFilter::DoFilter(const string &path)
{
        struct stat st; 

        if (lstat(path.c_str(), &st) < 0) {
                LOG_ERR("stat %s error: %s", path.c_str(), strerror(errno));
                return false;
        }
        date mTime = date_from_tm(*(localtime(&(st.st_mtim.tv_sec))));
        date now = date_from_tm(*(localtime(&m_TimeArg._TimeNow)));

        LOG_DBG("FAR[%d] NEAR[%d] FILE[%d] NOW[%d]", m_TimeArg._TimeBoundaryFar,
                m_TimeArg._TimeBoundaryNear, (int)mTime.day(), (int)now.day());

        /* 属于被指定的或者没有被过滤掉，进入下一次过滤条件判断 */
        if (m_TimeArg._Type == _DEMAND) {
                if ((now - mTime).days() <= m_TimeArg._TimeBoundaryFar && 
                    (now - mTime).days() >= m_TimeArg._TimeBoundaryNear) {
                        LOG_DBG("time filter ok: %s\n", path.c_str());
                        return m_FilterPtr->DoFilter(path);
                }
        }
        return false;
}
