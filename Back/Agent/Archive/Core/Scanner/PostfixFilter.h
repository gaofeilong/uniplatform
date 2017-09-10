/*******************************************************************************
 *
 * @ File Name  : PostfixFilter.h
 * @ Date       : 2012-10-17
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 
 * @ History    : 2012-10-17：创建
 *
 * ****************************************************************************/
#ifndef _POSTFIX_FILTER_H_
#define _POSTFIX_FILTER_H_

#include "Filter.h"

class PostfixFilter : public Filter {
public:
        PostfixFilter(BOOSTPTR<Filter> &ptr, const struct _FixFilterArg &arg);
        ~PostfixFilter();

public:
        bool DoFilter(const string &path);

private:
        BOOSTPTR<Filter> m_FilterPtr;
        struct _FixFilterArg m_PostfixArg;
};

#endif  //_POSTFIX_FILTER_H_
