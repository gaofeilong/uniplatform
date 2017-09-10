#include "Include/LvmConf.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/System.h"
#include "Agent/Nas/Share/ServiceAccess.h"
#include "Quota.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>

CQuota::CQuota()
{}

CQuota::~CQuota()
{}

int CQuota::GetMaxBlockSize(const std::string& path)
{
        int size = 0;
        int headsize = sizeof("/mnt/share");

        if ((int)path.size() <= headsize ) {
                return 0;
        }        
        std::string cmd = "df -Pk " + path;
        std::string result;

        if (!Popen(cmd.c_str(), result)) {
                LOG_ERR("Execmd (%s) error", cmd.c_str());
                return -1;
        }
        String::StringArray arr;
        String::Split(result.c_str(), '\n', arr);
        for(int i = 0; i < (int)arr.size(); i++)
        {
                if(strstr(arr[i].c_str(),path.c_str()) != NULL)
                {
                        String::StringArray arr2;
                        String::Split(arr[i].c_str(), ' ', arr2);
                        for(int j = 0; j < (int)arr2.size();j++)
                        {
                                size = atoi(arr2[j].c_str());
                                if( size != 0)
                                {
                                        break;
                                }
                        }
                }
        }
        size = size / 1024;

        return size;
}

int CQuota::GetMaxFileSize(const std::string& path)
{
        //return 102400;
        //一级目录下最大的文件数量
        return MAX_FILE_COUNT;
}

bool CQuota::GetQuotaDisk(String::StringArray& ary)
{
        std::string cmd = MAIN_PATH;
        cmd = cmd + PL_QUOTA + " disk";
        
        std::string result;
        if (!Popen(cmd.c_str(), result)) {
                LOG_ERR("Execute (%s) error", cmd.c_str());
                return false;
        }

        String::StringArray list;
        String::Split(result.c_str(), '\n', list);
        
        if (list.size() > 0){
                String::Split(list[0].c_str(), ':', ary);                
        }

        return true;
}

bool CQuota::GetUser(const std::string& path, QuotaList& list)
{
        std::string cmd = MAIN_PATH;
        cmd = cmd + PL_QUOTA + " user " + path;
        
        std::string result;
        if (!Popen(cmd.c_str(), result)) {
                LOG_ERR("Execute (%s) error", cmd.c_str());
                return false;
        }

        String::StringArray ary;;
        String::Split(result.c_str(), '\n', ary);

        for (uint i = 0; i < ary.size(); ++i)
        {
                Quota quo;
                String::StringArray ary1;
                String::Split(ary[i].c_str(), ':', ary1);
                
                if (ary1.size() < 4) {
                        LOG_ERR("size(%d) is error", ary1.size());
                        continue;
                }
                
                quo.id   = atoi(ary1[0].c_str());
                quo.name = ary1[1];
                quo.type = atoi(ary1[2].c_str());
                quo.bused = atol(ary1[3].c_str());
                quo.bsoft = atol(ary1[4].c_str());
                quo.bhard = atol(ary1[5].c_str());
                quo.fused = atol(ary1[6].c_str());
                quo.fsoft = atol(ary1[7].c_str());
                quo.fhard = atol(ary1[8].c_str());
                list.push_back(quo);
        }

        return true;
}

bool CQuota::GetGroup(const std::string& path, QuotaList& list)
{
        std::string cmd = MAIN_PATH;
        cmd = cmd + PL_QUOTA + " group " + path;
        
        std::string result;
        if (!Popen(cmd.c_str(), result)) {
                LOG_ERR("Execute (%s) error", cmd.c_str());
                return false;
        }

        String::StringArray ary;
        String::Split(result.c_str(), '\n', ary);

        for (uint i = 0; i < ary.size(); ++i)
        {
                String::StringArray ary1;
                Quota quo;
                String::Split(ary[i].c_str(), ':', ary1);
                
                if (ary1.size() < 4) {
                        LOG_ERR("size(%d) is error", ary1.size());
                        continue;
                }
                
                quo.id   = atoi(ary1[0].c_str());
                quo.name = ary1[1];
                quo.type = atoi(ary1[2].c_str());
                quo.bused = atol(ary1[3].c_str());
                quo.bsoft = atol(ary1[4].c_str());
                quo.bhard = atol(ary1[5].c_str());
                quo.fused = atol(ary1[6].c_str());
                quo.fsoft = atol(ary1[7].c_str());
                quo.fhard = atol(ary1[8].c_str());
                list.push_back(quo);
        }

        return true;
}

bool CQuota::SetUser(const std::string& path, const uint uid, const Quota& val)
{
        char cmd[1500];
        sprintf(cmd, "%s%s setuser %u %lu %lu %lu %lu %s", MAIN_PATH, PL_QUOTA, 
                uid, val.bsoft, val.bhard, val.fsoft, val.fhard, path.c_str());
        
        if (!ExeCmd(cmd)) {
                LOG_ERR("Execute (%s) error", cmd);
                return false;
        }
        
        return true;
}

bool CQuota::SetGroup(const std::string& path, const std::string& name, const Quota& val)
{
        char cmd[1500];
        sprintf(cmd, "%s%s setgroup %s %lu %lu %lu %lu %s", MAIN_PATH, PL_QUOTA, 
                name.c_str(), val.bsoft, val.bhard, val.fsoft, val.fhard, path.c_str());
        
        if (!ExeCmd(cmd)) {
                LOG_ERR("Execute (%s) error", cmd);
                return false;
        }
        
        return true;
}
