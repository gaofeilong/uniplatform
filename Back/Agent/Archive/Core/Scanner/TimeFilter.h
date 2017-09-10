/*******************************************************************************
 *
 * @ File Name  : TimeFilter.h
 * @ Date       : 2012-10-17
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 
 * @ History    : 2012-10-17：创建
 *
 * ****************************************************************************/
#ifndef _TIME_FILTER_H_
#define _TIME_FILTER_H_

#include "Filter.h"

class TimeFilter : public Filter {
public:
        TimeFilter(BOOSTPTR<Filter> &ptr, const struct _TimeFilterArg &arg);
        ~TimeFilter();

public:
        bool DoFilter(const string &path);

private:
        BOOSTPTR<Filter> m_FilterPtr;
        struct _TimeFilterArg m_TimeArg;
};

#endif  //_TIME_FILTER_H_
