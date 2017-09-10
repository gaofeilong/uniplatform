/*******************************************************************************
 *
 * @ File Name  : FilterFactory.cpp
 * @ Date       : 2012-10-24
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 
 * @ History    : 2012-10-24：创建
 *              : 2012-10-31：路径过滤标识敲错了
 *              : 2013-03-07：添加创建目录过滤对象层
 *
 * ****************************************************************************/
#include <stdio.h>

#include "FilterFactory.h"
#include "PostfixFilter.h"
#include "PrefixFilter.h"
#include "SizeFilter.h"
#include "TimeFilter.h"
#include "DirFilter.h"
#include "Utils/Log/Log.h"

/* constructor */
FilterFactory::FilterFactory()
{
}

/* destructor */
FilterFactory::~FilterFactory()
{
}

/**
 * FilterFactory() -工厂函数，根据过滤条件（结构体）创建过滤对象
 * @return: boost::shared_ptr指针，指向过滤对象
 */
BOOSTPTR<Filter> FilterFactory::CreateFilter(const struct _FilterArg &arg)
{
        BOOSTPTR<Filter> filterPtr = BOOSTPTR<Filter>(new Filter());
        LOG_DBG("create Filter\n");

        if (arg._SizeArg._Type != _NOFILTER) {
                filterPtr = BOOSTPTR<Filter>(new SizeFilter(filterPtr, arg._SizeArg));
                LOG_DBG("create SizeFilter\n");
        }
        if (arg._PostfixArg._Type != _NOFILTER) {
                filterPtr = BOOSTPTR<Filter>(new PostfixFilter(filterPtr, arg._PostfixArg));
                LOG_DBG("create PostfixFilter\n");
        }
        if (arg._PrefixArg._Type != _NOFILTER) {
                filterPtr = BOOSTPTR<Filter>(new PrefixFilter(filterPtr, arg._PrefixArg));
                LOG_DBG("create PrefixFilter\n");
        }
        if (arg._DirArg._Type != _NOFILTER) {
                filterPtr = BOOSTPTR<Filter>(new DirFilter(filterPtr, arg._DirArg));
                LOG_DBG("create DirFilter\n");
        }
        if (arg._TimeArg._Type != _NOFILTER) {
                filterPtr = BOOSTPTR<Filter>(new TimeFilter(filterPtr, arg._TimeArg));
                LOG_DBG("create TimeFilter\n");
        }
        return filterPtr;
}
