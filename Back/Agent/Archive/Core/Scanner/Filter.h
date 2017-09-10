/*******************************************************************************
 *
 * @ File Name  : Filter.h
 * @ Date       : 2012-10-17
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 
 * @ History    : 2012-10-17：创建
 *
 * ****************************************************************************/
#ifndef _FILTER_H_
#define _FILTER_H_

#include <boost/shared_ptr.hpp>

#include "FilterType.h"

#define BOOSTPTR        boost::shared_ptr

class Filter {
public:
        Filter();
        virtual ~Filter();

public:
        virtual bool DoFilter(const string &path);

};

#endif  //_FILTER_H_
