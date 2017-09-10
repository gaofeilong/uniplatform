/*******************************************************************************
 *
 * @ File Name  : PrefixFilter.h
 * @ Date       : 2012-10-17
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 
 * @ History    : 2012-10-17：创建
 *
 * ****************************************************************************/
#ifndef _PREFIX_FILTER_H_
#define _PREFIX_FILTER_H_

#include "Filter.h"

class PrefixFilter : public Filter {
public:
        PrefixFilter(BOOSTPTR<Filter> &ptr, const struct _FixFilterArg &arg);
        ~PrefixFilter();

private:
        bool DoFilter(const string &path);

private:
        BOOSTPTR<Filter> m_FilterPtr;
        struct _FixFilterArg m_PrefixArg;
};

#endif  //_PREFIX_FILTER_H_
