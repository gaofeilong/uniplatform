#include "Include/LvmConf.h"
#include "Utils/Log/Log.h"
#include "Agent/Nas/Base/ConfParser.h"
#include "Utils/Common/String.h"
#include "Utils/Shell/System.h"

#include "UserLdap.h"
#include <stdio.h>
#include <string.h>

static const std::string authConf = std::string(MAIN_PATH) + CONF_USERTYPE;

CUserLdap::CUserLdap()
{
        Init();
}

CUserLdap::~CUserLdap()
{}

bool CUserLdap::TestConnect()
{
        return true;
}

bool CUserLdap::Init()
{
        std::string path = MAIN_PATH;
        path += CONF_USERTYPE;
        CConfParser conf(path);

        conf.Load();
        
        conf.GetVal("[ldap]", "pwd", m_pwd);
        conf.GetVal("[ldap]", "basedn", m_baseDn);
        conf.GetVal("[ldap]", "rootdn", m_rootDn);

        conf.Clear();
        // m_rootDn = "cn=Manager,dc=my-domain,dc=com";
        // m_pwd = "111111";
        // m_baseDn = "dc=my-domain,dc=com";
        m_str = m_rootDn + " " + m_baseDn + " " + m_pwd + " ";
        return true;
}

bool CUserLdap::GetUserList(UserInfoList& list)
{
        char cmd[1500];
        sprintf(cmd ,"%s%s %s userlist", MAIN_PATH, PL_LDAP_OPER, m_str.c_str());
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

bool CUserLdap::GetUserList(UserInfoList& list, unsigned int gId)
{
        char cmd[1500];
        sprintf(cmd ,"%s%s %s groupuser %u", MAIN_PATH, PL_LDAP_OPER, m_str.c_str(), gId);
        std::string result;

        if (!Popen(cmd, result)) {
                LOG_ERR("Popen (%s) error", cmd);
                return false;
        }
        
        String::StringArray ary;
        String::Split(result.c_str(), ':', ary);

        for (uint i = 0; i < ary.size(); ++i)
        {
                UserInfo user;
                user.user = ary[i];
                list.push_back(user);
        }

        return true;
}

bool CUserLdap::AddUser(const UserInfo& info)
{
        char cmd[1500] = {0};
        uint maxuid = GetMaxUserID() + 1;
        if (maxuid < 500)
        {
                maxuid += 500;
        }

        do
        {
                sprintf(cmd, "%s%s %s adduser %s %u %u %s %s", MAIN_PATH, PL_LDAP_OPER, m_str.c_str(), 
                        info.user.c_str(), maxuid, info.gid, info.pwd.c_str(),info.userInfo.c_str());

                if (!ExeCmd(cmd)) {
                        LOG_ERR("Execmd(%s) error", cmd);
                        break;
                }
                
                sprintf(cmd, "%s%s %s %s", MAIN_PATH, PL_SMBPW, info.user.c_str(), info.pwd.c_str());
                if (!ExeCmd(cmd)) {
                        LOG_ERR("Execmd(%s) error", cmd);
                        break;
                }
                
                
                int ret=CNetUser::AddUser(info.user.c_str(), info.pwd.c_str(),info.gname.c_str());
                if(ret != 0)
                {
                        LOG_ERR("CNetUser::AddUser(%s) error", info.user.c_str());
                        break;
                }
                return true;
        } while (false);

        DelUser(info);

        return false;
}

bool CUserLdap::DelUser (const UserInfo& info, bool isReal)
{
        char cmd[1500] = {0};
        sprintf(cmd, "%s%s %s deluser %s", MAIN_PATH, PL_LDAP_OPER, m_str.c_str(), info.user.c_str());

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

bool CUserLdap::SetUserPwd(const UserInfo& info)
{
        char cmd[1500] = {0};
        sprintf(cmd, "%s%s %s chpwd %s %s", MAIN_PATH, PL_LDAP_OPER, m_str.c_str(), info.user.c_str(), info.pwd.c_str());

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
bool CUserLdap::Check(const UserInfo& info)
{
        return true;
}

bool CUserLdap::GetGroupList(GroupInfoList& list)
{
        char cmd[1500] = {0};
        sprintf(cmd, "%s%s %s grouplist", MAIN_PATH, PL_LDAP_OPER, m_str.c_str());
        std::string result;

        if (!Popen(cmd, result)) {
                LOG_ERR("Popen (%s) error", cmd);
                return false;
        }

        String::StringArray ary;
        String::Split(result.c_str(), '\n', ary);

        for (uint i = 0; i < ary.size(); ++i) {
                std::string& info = ary[i];
                String::StringArray ary1;
                String::Split(info.c_str(), ':', ary1);
                
                if (ary1.size() < 2) {
                        LOG_ERR("Split command count(%d) error", ary1.size());
                        return false;
                }
                
                GroupInfo group;
                group.group = ary1[0];
                group.gid   = atoi(ary1[1].c_str());
                group.groupInfo = ary1[2];

                list.push_back(group);
        }

        return true;
}

//group -g gid name tags
bool CUserLdap::AddGroup(const GroupInfo& info)
{
        char cmd[1500] = {0};
        uint maxgid = GetMaxGroupID() + 1;
        if (maxgid < 500)
        {
                maxgid += 500;
        }
        sprintf(cmd, "%s%s %s addgroup %s %u %s", MAIN_PATH, PL_LDAP_OPER, m_str.c_str(), 
                info.group.c_str(), maxgid, info.groupInfo.c_str());

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
bool CUserLdap::DelGroup(const GroupInfo& info)
{
        char cmd[1500] = {0};
        sprintf(cmd, "%s%s %s delgroup %s", MAIN_PATH, PL_LDAP_OPER, m_str.c_str(), info.group.c_str());
        
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

/*groupmod -G group user 
bool CUserLdap::AddUserToGroup(const std::string& user, const std::string& group, unsigned int uid, unsigned int gid)
{
        char cmd[1500] = {0};
        sprintf(cmd, "%s%s %s addgroupuser %s %s", MAIN_PATH, PL_LDAP_OPER, m_str.c_str(), group.c_str(), user.c_str());
        
        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                return false;
        }

        return true;
}

bool CUserLdap::DelUserFromGroup(const std::string& user, const std::string& group, unsigned int uid, unsigned int gid)
{
        char cmd[1500] = {0};
        sprintf(cmd, "%s%s %s delgroupuser %s %s", MAIN_PATH, PL_LDAP_OPER, m_str.c_str(), group.c_str(), user.c_str());
        
        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                return false;
        }

        return true;
}*/


uint CUserLdap::GetMaxGroupID()
{
        uint maxgid = 0;
        CUserOper::GroupInfoList list;
        bool bfound = GetGroupList(list);
        if(bfound)
        {
                for(uint i = 0; i < list.size(); i++)
                {
                        if(maxgid < list[i].gid)
                        {
                                maxgid = list[i].gid;
                        }
                }
               
        }
        return maxgid;
}

uint CUserLdap::GetMaxUserID()
{
        uint maxuid = 0;
        CUserOper::UserInfoList list;
        bool bfound = GetUserList(list);
        if(bfound)
        {
                for(uint i = 0; i < list.size(); i++)
                {
                        if(maxuid < list[i].uid)
                        {
                                maxuid = list[i].uid;
                        }
                }
               
        }
        return maxuid;
}

bool  CUserLdap::AddMultipleUsers(UserInfoList& list)
{
        bool bRet = true ;
        UserInfoList templist;
        for (uint i = 0; i < list.size(); i++)
        {
                UserInfo info = list[i];
                bool tempRet = CUserLdap::AddUser(info);
                if(!tempRet)
                {
                        LOG_ERR("Add user(%s) error", info.user.c_str());
                        templist.push_back(info);
                        bRet = tempRet;
                }
                sleep(1);
        }
        list.clear();
        list.assign(templist.begin(),templist.end());

        return bRet;

}

bool  CUserLdap::DelMultipleUsers(UserInfoList& list)
{
        bool bRet = true ;
        UserInfoList templist;
        for (uint i = 0; i < list.size(); i++)
        {
                UserInfo info;
                info = list[i];
                bool tempRet = CUserLdap::DelUser(info,true);
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

bool  CUserLdap::AddMultipleGroups(GroupInfoList & list)
{
        bool bRet = true ;
        GroupInfoList templist;
        for (uint i = 0; i < list.size(); i++)
        {
                GroupInfo info = list[i];
                bool tempRet = CUserLdap::AddGroup(info);
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

bool  CUserLdap::DelMultipleGroups(GroupInfoList& list)
{
        bool bRet = true ;
        GroupInfoList templist;
        for (uint i = 0; i < list.size(); i++)
        {
                GroupInfo info = list[i];
                bool tempRet = CUserLdap::DelGroup(info);
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
