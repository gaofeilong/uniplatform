#ifndef _DDFS_INFO_H_
#define _DDFS_INFO_H_

#include <string>
using std::string;

struct _DdfsPair {
        string id;
        string mp;
        string config;
};

struct _DedupInfo {
        string total;
        string real;
        string ratio;
};

#endif //_DDFS_INFO_H_
