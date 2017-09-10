/*******************************************************************************
 *
 * @ File Name  : SizeFilter.h
 * @ Date       : 2012-10-17
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 
 * @ History    : 2012-10-17：创建
 *
 * ****************************************************************************/
#ifndef _SIZE_FILTER_H_
#define _SIZE_FILTER_H_

#include "Filter.h"

class SizeFilter : public Filter {
public:
        SizeFilter(BOOSTPTR<Filter> &ptr, const struct _SizeFilterArg &arg);
        ~SizeFilter();

public:
        bool DoFilter(const string &path);

private:
        inline bool InRange(size_t size);

private:
        BOOSTPTR<Filter> m_FilterPtr;
        struct _SizeFilterArg m_SizeArg;
};

#endif  //_SIZE_FILTER_H_
