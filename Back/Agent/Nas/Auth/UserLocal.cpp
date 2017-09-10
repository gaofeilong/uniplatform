#include "Include/LvmConf.h"
#include "Include/Constant.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/String.h"
#include "Utils/Shell/System.h"

#include "UserLocal.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>

CUserLocal::CUserLocal()
{}

CUserLocal::~CUserLocal()
{}

//1、组名，2、密码（一般空）3、gid，4、组内用户列表，逗号隔开
//1、用户名，2、密码3、uid，4、gid5、用户全名6、开始目录7、shell工具
bool CUserLocal::GetUserList(UserInfoList& list)
{
        char cmd[1500];
        sprintf(cmd ,"awk -F\":\" '$3>=500 && $3 <= 4294967293 && $3!=65534 {print $1\":\"$3\":\"$4\":\"$5}' /etc/passwd");
        std::string result;
        
        if (!Popen(cmd, result)) {
                LOG_ERR("Popen (%s) error", cmd);
                return false;
        }

        String::StringArray ary;
        String::Split(result.c_str(), '\n', ary);

        GroupInfoList groupList;
        if (!GetGroupList(groupList)) {
                LOG_ERR("GetGroupList error");
                return false;
        }

        for (uint i = 0; i < ary.size(); ++i)
        {
                std::string& info = ary[i];
                String::StringArray ary1;
                String::Split(info.c_str(), ':', ary1);
                
                if (ary1.size() < 3) {
                        LOG_ERR("Split command count(%d) error", ary1.size());
                        return false;
                }
                
                UserInfo user;
                user.uid = atoi(ary1[1].c_str());
                user.user = ary1[0];
                user.gid = atoi(ary1[2].c_str());
                if (ary1.size() == 4) {
                        user.userInfo = ary1[3];
                }
                list.push_back(user);
        }

        return true;
}

bool CUserLocal::GetUserList(UserInfoList& list, unsigned int gId)
{
        char cmd[1500];
        sprintf(cmd ,"awk -F\":\" '$3==%u {print $4}' /etc/group", gId);
        std::string result;

        if (!Popen(cmd, result)) {
                LOG_ERR("Popen (%s) error", cmd);
                return false;
        }

        String::StringArray ary1, ary2;

        String::Split(result.c_str(), '\n', ary1);

        for (uint i = 0; i < ary1.size(); ++i)
        {
                String::Split(ary1[i].c_str(), ',', ary2);

                for (uint j = 0; j < ary2.size(); ++j) 
                {
                        UserInfo user;
                        user.user = ary2[j];
                        list.push_back(user);
                }
        }
        

        return true;
}

bool CUserLocal::AddUser(const UserInfo& info)
{
        bool bRet = true;
        char cmd[1500] = {0};
        sprintf(cmd, "useradd -M -c '%s' -g %u %s -s /sbin/nologin", info.userInfo.c_str(), info.gid,info.user.c_str());

        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                bRet = false;
        }
                
        if(bRet)
        {
                memset(cmd, 0, 1500);
                //echo "view:123456"| chpasswd
                sprintf(cmd, "echo \"%s:%s\" | chpasswd", info.user.c_str(), info.pwd.c_str());
                if (!ExeCmd(cmd)) 
                {
                        LOG_ERR("Execmd(%s) error", cmd);
                        bRet = false;
                }
        }

        if(bRet)
        {
                memset(cmd, 0, 1500);
                sprintf(cmd, "%s%s %s %s", MAIN_PATH, PL_SMBPW, info.user.c_str(), info.pwd.c_str());
                if (!ExeCmd(cmd)) 
                {
                        LOG_ERR("Execmd(%s) error", cmd);
                        bRet = false;
                }
        }
        if(bRet)
        {
                int ret=CNetUser::AddUser(info.user.c_str(), info.pwd.c_str(),info.gname.c_str());
                if(ret != 0)
                {
                        LOG_ERR("CNetUser::AddUser(%s) error", info.user.c_str());
                        bRet = false;
                }
        }

        if(!bRet)
        {
                CUserLocal::DelUser(info);
        }
        return bRet;
}

bool CUserLocal::DelUser (const UserInfo& info, bool isReal)
{
        char cmd[1500] = {0};
        sprintf(cmd, "userdel %s", info.user.c_str());

        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                return false;
        }
        
        memset(cmd,0,1500);
        sprintf(cmd, "smbpasswd -x %s", info.user.c_str());

        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                return false;
        }
        CUserOper::EmptyQuota(info.uid,0);
                
        int ret=CNetUser::DelUser(info.user.c_str());
        if(ret != 0)
        {
                LOG_ERR("CNetUser::DelUser(%s) error", info.user.c_str());
                return false;
        }

        return true;
}

bool CUserLocal::SetUserPwd(const UserInfo& info)
{
        char cmd[1500] = {0};
        //sprintf(cmd, "usermod -p '%s' %s", info.pwd.c_str(), info.user.c_str());
        sprintf(cmd, "echo \"%s:%s\" | chpasswd", info.user.c_str(), info.pwd.c_str());
        
        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                return false;
        }

        sprintf(cmd, "%s%s %s %s", MAIN_PATH, PL_SMBPW, info.user.c_str(), info.pwd.c_str());
        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                return false;
        }
        
        int ret=CNetUser::SetPwd(info.user.c_str(),info.pwd.c_str());
        if(ret != 0)
        {
                LOG_ERR("CNetUser::SetPwd(%s) error", info.user.c_str());
                return false;
        }
        
        return true;
}

//
bool CUserLocal::Check(const UserInfo& info)
{
        return true;
}

//1、组名，2、密码（一般空）3、gid，4、组内用户列表，逗号隔开
bool CUserLocal::GetGroupList(GroupInfoList& list)
{
        char cmd[1500] = {0};
        sprintf(cmd, "awk -F\":\" '$3>=500 && $3 <= 4294967293 && $3!=65534 {print $1\":\"$3\":\"$4}' /etc/group");
        std::string result;

        if (!Popen(cmd, result)) {
                LOG_ERR("Popen (%s) error", cmd);
                return false;
        }

        String::StringArray ary;
        String::Split(result.c_str(), '\n', ary);

        for (uint i = 0; i < ary.size(); ++i)
        {
                std::string& info = ary[i];
                String::StringArray ary1;
                String::Split(info.c_str(), ':', ary1);
                
                if (ary1.size() < 2) {
                        LOG_ERR("Split command count(%d) error", ary1.size());
                        return false;
                }
                
                GroupInfo group;
                group.group = ary1[0];
                group.gid = atoi(ary1[1].c_str());
                
                list.push_back(group);
        }

        return true;
}

//groupadd name
bool CUserLocal::AddGroup(const GroupInfo& info)
{
        char cmd[1500] = {0};
        sprintf(cmd, "groupadd %s",info.group.c_str());

        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                return false;
        }
        
        int ret=CNetUser::AddGroup(info.group.c_str());
        if(ret != 0)
        {
                LOG_ERR("CNetUser::AddGroup(%s) error", info.group.c_str());
                return false;
        }
        return true;
}

// groupdel name
bool CUserLocal::DelGroup(const GroupInfo& info)
{
        char cmd[1500] = {0};
        sprintf(cmd, "groupdel %s", info.group.c_str());
        
        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                return false;
        }
        
        CUserOper::EmptyQuota(info.gid,1);

        int ret=CNetUser::DelGroup(info.group.c_str());
        if(ret != 0)
        {
                LOG_ERR("CNetUser::DelGroup(%s) error", info.group.c_str());
                return false;
        }
        
       return true;
}

/*//groupmod -G 
bool CUserLocal::AddUserToGroup(const std::string& user, const std::string& group, unsigned int uid, unsigned int gid)
{
        char cmd[1500] = {0};
        sprintf(cmd, "usermod -a -G %u %s", gid, user.c_str());
        
        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                return false;
        }

        return true;
}

bool CUserLocal::DelUserFromGroup(const std::string& user, const std::string& group, unsigned int uid, unsigned int gid)
{
        // 查找该用户所在的所有组id列表
        char cmd[1500] = {0};
        sprintf(cmd, "grep -w %s /etc/group | awk -F \":\" '$1 != %s {print$3}' | grep -vw %u", user.c_str(), user.c_str(), gid);

        std::string result;
        if (!Popen(cmd, result)) {
                LOG_ERR("Execmd(%s) error", cmd);
                return false;
        }

        // 去掉行尾换行符
        if (result.size() > 1 && result[result.size()-1] == '\n') {
                result[result.size()-1] = 0;
        }

        // 去掉行中\n为，执行命令
        std::replace(result.begin(), result.end(), '\n', ',');
        sprintf(cmd, "usermod -G %s %s", result.c_str(), user.c_str());
        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                return false;
        }

        return true;
}*/

bool  CUserLocal::AddMultipleUsers(UserInfoList& list)
{
        bool bRet = true ;
        UserInfoList templist;
        for (uint i = 0; i < list.size(); i++)
        {
                UserInfo info = list[i];
                bool tempRet = CUserLocal::AddUser(info);
                if(!tempRet)
                {                          
                        LOG_ERR("Add user(%s) error", info.user.c_str());
                        templist.push_back(info);
                        bRet = tempRet;
                }                
        }
        list.clear();
        list.assign(templist.begin(),templist.end());
        
        return bRet;
        
}

bool  CUserLocal::DelMultipleUsers(UserInfoList& list)
{
        bool bRet = true ;
        UserInfoList templist;
        for (uint i = 0; i < list.size(); i++)
        {
                UserInfo info;
                info = list[i];
                bool tempRet = CUserLocal::DelUser(info,true);
                if(!tempRet)
                {                          
                        LOG_ERR("Del user(%s) error", info.user.c_str());
                        templist.push_back(info);
                        bRet = tempRet;
                }            
        }
        
        list.clear();
        list.assign(templist.begin(),templist.end());
        
        return bRet;
        
}

bool  CUserLocal::AddMultipleGroups(GroupInfoList & list)
{
        bool bRet = true ;
        GroupInfoList templist;
        for (uint i = 0; i < list.size(); i++)
        {
                GroupInfo info = list[i];
                bool tempRet = CUserLocal::AddGroup(info);
                if(!tempRet)
                {        
                        LOG_ERR("Add group(%s) error", info.group.c_str());
                        templist.push_back(info);
                        bRet = tempRet;
                }                
        }
        
        list.clear();
        list.assign(templist.begin(),templist.end());
        
        return bRet;
        
}

bool  CUserLocal::DelMultipleGroups(GroupInfoList& list)
{
        bool bRet = true ;
        GroupInfoList templist;
        for (uint i = 0; i < list.size(); i++)
        {
                GroupInfo info = list[i];
                bool tempRet = CUserLocal::DelGroup(info);
                if(!tempRet)
                {        
                        LOG_ERR("Del group(%s) error", info.group.c_str());
                        templist.push_back(info);
                        bRet = tempRet;
                }                
        }
        
        list.clear();
        list.assign(templist.begin(),templist.end());
        
        return bRet;
}
