#ifndef __USER_LDAP_H__
#define __USER_LDAP_H__

#include "UserOper.h"

class CUserLdap : public CUserOper
{
public:
        CUserLdap();
        virtual ~CUserLdap();

        //用户操作
        //获取用户列表
        bool GetUserList(UserInfoList& list);
        //获取指定组的用户列表
        bool GetUserList(UserInfoList& list, unsigned int gId);
        //添加新用户
        bool AddUser(const UserInfo& info);
        //删除用户,isReal是否真实删除
        bool DelUser (const UserInfo& info, bool isReal = false);
        //修改用户密码
        bool SetUserPwd(const UserInfo& info);
        //确认用户名及密码
        bool Check(const UserInfo& info);

        //组操作
        //获取组列表
        bool GetGroupList(GroupInfoList& list);
        //添加新组
        bool AddGroup(const GroupInfo& info);
        //删除组
        bool DelGroup(const GroupInfo& info);

        //用户&组混合操作
        //向组内添加用户,该操作不会真实的删除用户
        //bool AddUserToGroup(const std::string& user, const std::string& group, unsigned int uid, unsigned int gid);
        //组内删除指定用户
        //bool DelUserFromGroup(const std::string& user, const std::string& group, unsigned int uid, unsigned int gid);
        
        //批量添加用户
        bool  AddMultipleUsers(UserInfoList& list);
        
        //批量删除用户
        bool  DelMultipleUsers(UserInfoList& list);
        
        //批量添加用户组
        bool  AddMultipleGroups(GroupInfoList & list);
        
        //批量删除用户组
        bool  DelMultipleGroups(GroupInfoList& list);
        
private:
        bool TestConnect();
        bool Init();
        //获取最大组ID
        uint GetMaxGroupID();
        
        //获取最大用户ID
        uint GetMaxUserID();
        
private:
        std::string m_rootDn;
        std::string m_pwd;
        std::string m_baseDn;
        std::string m_ip;
        std::string m_str;
        
        bool m_tls;
};

#endif /* __USER_LDAP_H__ */
