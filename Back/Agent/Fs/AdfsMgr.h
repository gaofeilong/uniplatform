/*******************************************************************************
 *
 * @ File Name  : AdfsMgr.h
 * @ Date       : 2013-10-29
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: Adfs 文件系统管理
 * @ History    : 2013-10-29：创建
 *
 * ****************************************************************************/

#ifndef _ADFS_MGR_H_
#define _ADFS_MGR_H_

#include "FsMgr.h"

#include <stdint.h>

#include <string>
#include <vector>

using std::string;
using std::vector;

struct _AdfsInfo {
        string firstDataPath;
        vector<string> otherDataPath;
        string capacity;
        string left;
        string total;
        string real;
        string ratio;
        string state;
};

class AdfsMgr: public FsMgr {
public:
        AdfsMgr();
        ~AdfsMgr();

public:
        int GetMpList(vector<struct _AdfsInfo> &mpList);
        int Create(const string &firstDataPath, 
                   const vector<string> otherDataPath);
        int Modify(const string &firstDataPath, 
                   const vector<string> newDataPath);
        int Mount(const string &firstDataPath);
        int Umount(const string &firstDataPath);
        int Remove(const string &firstDataPath);
        string GetMpStatus(const string &mp);

private:
        int GetAllDataPath(vector<struct _AdfsInfo> &mpList);
        int SetAllDataPath(const vector<struct _AdfsInfo> &mpList);
        int GetDataPathKey(const string &firstDataPath, string &pathKey);
        bool IsPathListValid(const vector<string> &newPathList,
                             const vector<string> &oldPathList);
};

#endif  //_ADFS_MGR_H_
