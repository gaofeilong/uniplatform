/*******************************************************************************
 *
 * @ File Name  : DirFilter.h
 * @ Date       : 2012-10-17
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 过滤文件目录
 * @ History    : 2012-10-17：创建
 *
 * ****************************************************************************/
#ifndef _DIR_FILTER_H_
#define _DIR_FILTER_H_

#include "Filter.h"

class DirFilter : public Filter {
public:
        DirFilter(BOOSTPTR<Filter> &ptr, const struct _DirFilterArg &arg);
        ~DirFilter();

private:
        bool DoFilter(const string &path);

private:
        BOOSTPTR<Filter> m_FilterPtr;
        struct _DirFilterArg m_DirArg;
};

#endif  //_DIR_FILTER_H_
