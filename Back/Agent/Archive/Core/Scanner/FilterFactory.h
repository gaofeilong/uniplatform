/*******************************************************************************
 *
 * @ File Name  : FilterFactory.h
 * @ Date       : 2012-10-24
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 
 * @ History    : 2012-10-24：创建
 *
 * ****************************************************************************/
#ifndef _FILTER_FACTORY_H_
#define _FILTER_FACTORY_H_

#include "FilterType.h"
#include "Filter.h"

class FilterFactory {
public:
        FilterFactory();
        ~FilterFactory();

public:
        BOOSTPTR<Filter> CreateFilter(const struct _FilterArg &arg);

};

#endif  //_FILTER_FACTORY_H_
