#include "Include/LvmConf.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/String.h"
#include "Utils/Shell/System.h"
#include "Agent/Nas/Share/ServiceAccess.h"

#include "UserNIS.h"
#include <stdio.h>


CUserNIS::CUserNIS()
{
}

CUserNIS::~CUserNIS()
{}


bool CUserNIS::GetUserList(UserInfoList& list)
{
        char cmd[128] = {0};
        sprintf(cmd ,"ypcat passwd");
        std::string result;
        
        if (!Popen(cmd, result)) {
                LOG_ERR("Popen (%s) error", cmd);
                return false;
        }

        String::StringArray arr;
        String::Split(result.c_str(), '\n', arr);

        for (uint i = 0; i < arr.size(); i++)
        {
                String::StringArray arr2;
                String::Split(arr[i].c_str(), ':', arr2);
                
                UserInfo user;
                
                user.user = arr2[0];
                user.uid = atoi(arr2[2].c_str());
                user.gid = atoi(arr2[3].c_str());
                user.issetpwd = 0;
                list.push_back(user);
        }

        UserInfoList lastlist;
        CUserOper::GetLastUsers(lastlist);
        
        //减少的用户
        for(uint i = 0; i < lastlist.size(); i++)
        {
                bool bfound = false;
                UserInfo user = lastlist[i];
                for(uint j = 0; j < list.size(); j++)
                {
                        if(list[j].user == user.user)
                        {
                                bfound = true;
                                break;
                        }
                }       
                if(!bfound)
                {
                        CNetUser::DelUser(user.user.c_str());
                        
                        std::vector<std::string> ulist;
                        ulist.push_back(user.user);
                        CServiceAccess::RemoveUser(ulist);
                                
                        char cmd2[1024] = {0};
                        sprintf(cmd2, "smbpasswd -x %s", user.user.c_str());

                        if (!ExeCmd(cmd2)) {
                                LOG_ERR("Execmd(%s) error", cmd2);
                        }
                        CUserOper::EmptyQuota(user.uid,0);
                }
        }
        
        //查找用户的issetpwd
        for(uint i = 0; i < list.size(); i++)
        {
                bool bfound = false;
                UserInfo user = list[i];
                for(uint j = 0; j < lastlist.size(); j++)
                {
                        if(lastlist[j].user == user.user)
                        {
                                list[i].issetpwd = lastlist[j].issetpwd;
                                bfound = true;
                                break;
                        }
                }/*
                if(!bfound)
                {
                        CNetUser::AddUser(user.user.c_str());
                }*/
        }
        
        CUserOper::UpdatetoLastUsers(list);
        return true;
}

bool CUserNIS::GetUserList(UserInfoList& list, unsigned int gId)
{
        char cmd[128] = {0};
        sprintf(cmd ,"ypcat passwd");
        std::string result;
        
        if (!Popen(cmd, result)) {
                LOG_ERR("Popen (%s) error", cmd);
                return false;
        }

        String::StringArray arr;
        String::Split(result.c_str(), '\n', arr);

        for (uint i = 0; i < arr.size(); i++)
        {
                String::StringArray arr2;
                String::Split(arr[i].c_str(), ':', arr2);
                
                
                if((int)gId == atoi(arr2[3].c_str()))
                {
                        UserInfo user;
                        user.user = arr2[0];
                        user.uid = atoi(arr2[2].c_str());
                        user.gid = atoi(arr2[3].c_str());
                        user.issetpwd = 0;
                        list.push_back(user);
                }
        }
        return true;
}

bool CUserNIS::GetGroupList(GroupInfoList& list)
{
        char cmd[128] = {0};
        sprintf(cmd ,"ypcat group");
        std::string result;
        
        if (!Popen(cmd, result)) {
                LOG_ERR("Popen (%s) error", cmd);
                return false;
        }

        String::StringArray arr;
        String::Split(result.c_str(), '\n', arr);

        for (uint i = 0; i < arr.size(); i++)
        {
                String::StringArray arr2;
                String::Split(arr[i].c_str(), ':', arr2);
                
                GroupInfo group;
                
                group.group = arr2[0];
                group.gid = atoi(arr2[2].c_str());
                list.push_back(group);
        }

        GroupInfoList lastlist;
        CUserOper::GetLastGroups(lastlist);
        
        //减少的用户组
        for(uint i = 0; i < lastlist.size(); i++)
        {
                bool bfound = false;
                GroupInfo group = lastlist[i];
                for(uint j = 0; j < list.size(); j++)
                {
                        if(list[j].group == group.group)
                        {
                                bfound = true;
                                break;
                        }
                }       
                if(!bfound)
                {
                        CNetUser::DelGroup(group.group.c_str());
                        CUserOper::EmptyQuota(group.gid,1);
                }
        }
        
        //增加的用户组
        for(uint i = 0; i < list.size(); i++)
        {
                bool bfound = false;
                GroupInfo group = list[i];
                for(uint j = 0; j < lastlist.size(); j++)
                {
                        if(lastlist[j].group == group.group)
                        {
                                bfound = true;
                                break;
                        }
                }
                if(!bfound)
                {
                        CNetUser::AddGroup(group.group.c_str());
                }
        }
        
        CUserOper::UpdatetoLastGroups(list);
        return true;
}

bool CUserNIS::SetUserPwd(const UserInfo& info)
{
        char cmd[1500] = {0};
        
        sprintf(cmd, "%s%s %s %s", MAIN_PATH, PL_SMBPW, info.user.c_str(), info.pwd.c_str());
        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                return false;
        }
        
        int Ret = CNetUser::AddUser(info.user.c_str(),info.pwd.c_str());
        if(Ret == 0)
        {
                UserInfo tmpinfo;
                tmpinfo.user = info.user;
                tmpinfo.pwd = info.pwd;
                tmpinfo.issetpwd = 1;
                CUserOper::UpdateUser(tmpinfo);
                return true;
        }
        return false;
}
