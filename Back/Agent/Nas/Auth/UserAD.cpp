#include "Utils/Log/Log.h"
#include "Utils/Common/String.h"
#include "Utils/Shell/System.h"
#include "Agent/Nas/Share/ServiceAccess.h"

#include "UserAD.h"
#include <stdio.h>

CUserAD::CUserAD()
{        
}

CUserAD::~CUserAD()
{}

bool CUserAD::GetUserList(UserInfoList& list)
{
        //wbinfo --user-info=guest
        //guest:*:16777219:16777226:Guest:/home/SCINAS/guest:/bin/bash
        
        char cmd[128] = {0};
        sprintf(cmd ,"wbinfo -u | grep -v '\\\\'");
        std::string result;
        
        if (!Popen(cmd, result)) {
                LOG_ERR("Popen (%s) error", cmd);
                return false;
        }

        String::StringArray arr;
        String::Split(result.c_str(), '\n', arr);

        for (uint i = 0; i < arr.size(); i++)
        {
                std::string temp_user = arr[i];
                char cmd2[128] = {0};
                sprintf(cmd2 ,"wbinfo --user-info=%s",temp_user.c_str());
                std::string result2;
                
                if (!Popen(cmd2, result2)) {
                        LOG_ERR("Popen (%s) error", cmd2);
                        continue;
                }
                if(result2.find(":",0) != std::string::npos )
                {
                        String::StringArray arr2;
                        String::Split(result2.c_str(), ':', arr2);
                
                        UserInfo user;
                
                        user.user = arr2[0];
                        user.uid = atoi(arr2[2].c_str());
                        user.gid = atoi(arr2[3].c_str());
                        user.issetpwd = 0;
                        list.push_back(user);
                }
        }

        CUserOper::UserInfoList lastlist;
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
                        CUserOper::EmptyQuota(user.uid,0);
                        CNetUser::DelUser(user.user.c_str());

                        std::vector<std::string> ulist;
                        ulist.push_back(user.user);
                        CServiceAccess::RemoveUser(ulist);
                }
        }
        
        //增加的用户
        for(uint i = 0; i < list.size(); i++)
        {
                bool bfound = false;
                UserInfo user = list[i];
                for(uint j = 0; j < lastlist.size(); j++)
                {
                        if(lastlist[j].user == user.user)
                        {
                                //原有的用户可能设置过密码
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

bool CUserAD::SetUserPwd(const UserInfo& info)
{
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
