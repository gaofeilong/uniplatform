/*******************************************************************************
 *
 * @ File Name  : DirFilter.cpp
 * @ Date       : 2013-3-7
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 指定、过滤文件目录
 * @ History    : 2013-3-7：创建
 *
 * ****************************************************************************/
#include "DirFilter.h"

/* constructor */
DirFilter::DirFilter(BOOSTPTR<Filter> &ptr, 
                        const struct _DirFilterArg &arg): 
        m_FilterPtr(ptr), m_DirArg(arg)
{
}

/* destructor */
DirFilter::~DirFilter()
{
}

/**
 * DoFilter() -归档子路径过滤函数，继承自 Filter 类
 * @path: 被检查文件路径
 * @arg: 过滤条件结构体
 * @return: true 符合条件
 *          false 不符合条件 
 */
bool DirFilter::DoFilter(const string &path)
{
        set<string>::const_iterator ssCit = m_DirArg._DirSet.begin(); 

        if (m_DirArg._Type == _DEMAND) {
                for (; ssCit != m_DirArg._DirSet.end(); ++ssCit) {
                        if (path.find(*ssCit) == 0) {
#ifdef  _DEBUG_
                                LOG_DBG("dir filter ok: %s\n", path.c_str());
#endif  //_DEBUG_
                                return m_FilterPtr->DoFilter(path);
                        }
                }
#ifdef  _DEBUG_
                                LOG_DBG("dir filter error: %s\n", path.c_str());
#endif  //_DEBUG_
                return false;
        } else {
                for (; ssCit != m_DirArg._DirSet.end(); ++ssCit) {
                        if (path.find(*ssCit) == 0) {
#ifdef  _DEBUG_
                                LOG_DBG("dir filter error: %s\n", path.c_str());
#endif  //_DEBUG_
                                return false;
                        }
                }
#ifdef  _DEBUG_
                LOG_DBG("dir filter ok: %s\n", path.c_str());
#endif  //_DEBUG_
                return m_FilterPtr->DoFilter(path);
        }
        
}
